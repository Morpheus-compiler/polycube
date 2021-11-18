#include <uapi/linux/bpf.h>
#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/udp.h>
#include <uapi/linux/tcp.h>

#include "BMC_common.h"

BPF_TABLE_SHARED("array", int, struct bmc_cache_entry, map_kcache, BMC_CACHE_ENTRY_COUNT);
BPF_TABLE_SHARED("percpu_array", int, struct memcached_key, map_keys, BMC_MAX_KEY_IN_PACKET);
BPF_TABLE_SHARED("percpu_array", int, struct parsing_context, map_parsing_context, 1);
BPF_TABLE_SHARED("percpu_array", int, struct bmc_stats, map_stats, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_rx_filter, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_hash_keys, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_prepare_packet, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_invalidate_cache, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_update_cache, 1);
BPF_TABLE_SHARED("percpu_array", int, uint64_t, pkt_cnt_write_reply, 1);

//BPF_TABLE("extern", int, struct elements, sharedEle, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	void *transp = data + sizeof(*eth) + sizeof(*ip);
	struct udphdr *udp;
	struct tcphdr *tcp;
	char *payload;
	__be16 dport;

	pcn_log(ctx, LOG_TRACE, "[RX filter] Packet received");

	int idx = 0;
	uint64_t *cnt = pkt_cnt_rx_filter.lookup(&idx);
	if (!cnt) {
		return RX_DROP;		
	}

	NO_TEAR_INC(*cnt);
	
	if (ip + 1 > data_end)
		return RX_OK;

	switch (ip->protocol) {
		case IPPROTO_UDP:
			udp = (struct udphdr *) transp;
			if (udp + 1 > data_end)
				return RX_OK;
			dport = udp->dest;
			payload = transp + sizeof(*udp) + sizeof(struct memcached_udp_header);
			break;
		case IPPROTO_TCP:
			tcp = (struct tcphdr *) transp;
			if (tcp + 1 > data_end)
				return RX_OK;
			dport = tcp->dest;
			payload = transp + sizeof(*tcp);
			break;
		default:
			return RX_OK;
	}

	if (dport == htons(11211) && payload + 4 <= data_end) {
		pcn_log(ctx, LOG_TRACE, "[RX filter] Dst port is 11211 and there is some payload");

		if (ip->protocol == IPPROTO_UDP && payload[0] == 'g' && payload[1] == 'e' && payload[2] == 't' && payload[3] == ' ') { // is this a GET request
			pcn_log(ctx, LOG_TRACE, "[RX filter] Received GET request on RX filter from IP: %I", ip->saddr);
			unsigned int zero = 0;
			struct bmc_stats *stats = map_stats.lookup(&zero);
			if (!stats) {
				return RX_OK;
			}
			stats->get_recv_count++;

			struct parsing_context *pctx = map_parsing_context.lookup(&zero);
			if (!pctx) {
				return RX_OK;
			}
			pctx->key_count = 0;
			pctx->current_key = 0;
			pctx->write_pkt_offset = 0;

			unsigned int off;
#pragma clang loop unroll(disable)
			for (off = 4; off < BMC_MAX_PACKET_LENGTH && payload+off+1 <= data_end && payload[off] == ' '; off++) {} // move offset to the start of the first key
			if (off < BMC_MAX_PACKET_LENGTH) {
				pctx->read_pkt_offset = off; // save offset
				if (bpf_xdp_adjust_head(ctx, (int)(sizeof(*eth) + sizeof(*ip) + sizeof(*udp) + sizeof(struct memcached_udp_header) + off))) { // push headers + 'get ' keyword
					return RX_OK;
				}
				// NO_TEAR_INC(*cnt);
                call_ingress_program(ctx, BMC_PROG_XDP_HASH_KEYS);
			}
		}
		else if (ip->protocol == IPPROTO_TCP) {
			pcn_log(ctx, LOG_TRACE, "[RX filter] Packet is not UDP or not a GET, go to Invalidate Cache");
			// NO_TEAR_INC(*cnt);
            call_ingress_program(ctx, BMC_PROG_XDP_INVALIDATE_CACHE);
		}
	}

	// NO_TEAR_INC(*cnt);
	return RX_OK;
}