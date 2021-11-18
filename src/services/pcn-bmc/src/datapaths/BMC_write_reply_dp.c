#include <uapi/linux/bpf.h>
#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/udp.h>
#include <uapi/linux/tcp.h>

#include "BMC_common.h"

BPF_TABLE("extern", int, struct bmc_cache_entry, map_kcache, BMC_CACHE_ENTRY_COUNT);
BPF_TABLE("extern", int, struct memcached_key, map_keys, BMC_MAX_KEY_IN_PACKET);
BPF_TABLE("extern", int, struct parsing_context, map_parsing_context, 1);
BPF_TABLE("extern", int, struct bmc_stats, map_stats, 1);
BPF_TABLE("extern", int, uint64_t, pkt_cnt_write_reply, 1);

static inline u16 compute_ip_checksum(struct iphdr *ip)
{
    u32 csum = 0;
    u16 *next_ip_u16 = (u16 *)ip;

    ip->check = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < (sizeof(*ip) >> 1); i++) {
        csum += *next_ip_u16++;
    }

	return ~((csum & 0xffff) + (csum >> 16));
}

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	char *payload = (char *) data;
	unsigned int zero = 0;

	pcn_log(ctx, LOG_TRACE, "[Write Reply] Packet received");
	// bpf_trace_printk("[Write Reply] Packet received");

	if (payload >= data_end)
		return RX_OK;

	struct parsing_context *pctx = map_parsing_context.lookup(&zero);
	if (!pctx) {
		return RX_OK;
	}

	struct memcached_key *key = map_keys.lookup(&pctx->current_key);
	if (!key) {
		return RX_OK;
	}

	unsigned int cache_hit = 1, written = 0;
	u32 cache_idx = key->hash % BMC_CACHE_ENTRY_COUNT;
	// struct bmc_cache_entry *entry = map_kcache.lookup(&cache_idx);
	struct bmc_cache_entry *entry = map_kcache.lookup(&cache_idx);
	if (!entry) {
		return RX_OK;
	}

	// //bpf_spin_lock(&entry->lock);
	if (entry->valid && key->hash == entry->hash) { // if saved key still matches its corresponding cache entry
		pcn_log(ctx, LOG_TRACE, "[Write Reply] Entry is still valid");
#pragma clang loop unroll(disable)
		for (int i = 0; i < BMC_MAX_KEY_LENGTH && i < key->len; i++) { // compare the saved key with the one stored in the cache entry
			if (key->data[i] != entry->data[6+i]) {
				cache_hit = 0;
			}
		}
		if (cache_hit) { // if cache HIT then copy cached data
			pcn_log(ctx, LOG_TRACE, "[Write Reply] Copy cached data");
			unsigned int off;
#pragma clang loop unroll(disable)
			for (off = 0; off+sizeof(unsigned long long) < BMC_MAX_CACHE_DATA_SIZE && off+sizeof(unsigned long long) <= entry->len && payload+off+sizeof(unsigned long long) <= data_end; off++) {
				*((unsigned long long *) &payload[off]) = *((unsigned long long *) &entry->data[off]);
				off += sizeof(unsigned long long)-1;
				written += sizeof(unsigned long long);
			}
#pragma clang loop unroll(disable)
			for (; off < BMC_MAX_CACHE_DATA_SIZE && off < entry->len && payload+off+1 <= data_end; off++) {
				payload[off] = entry->data[off];
				written += 1;
			}
		}
	}
	// //bpf_spin_unlock(&entry->lock);

	struct bmc_stats *stats = map_stats.lookup(&zero);
	if (!stats) {
		return RX_OK;
	}
	if (cache_hit) {
		pcn_log(ctx, LOG_TRACE, "[Write Reply] Update hit count");
		stats->hit_count++;
	} else {
		pcn_log(ctx, LOG_TRACE, "[Write Reply] Update miss count");
		stats->miss_count++;
	}

	uint64_t *cnt = pkt_cnt_write_reply.lookup(&zero);
	if (!cnt) {
		return RX_DROP;		
	}

	pctx->current_key++;

	if (pctx->current_key == pctx->key_count && (pctx->write_pkt_offset > 0 || written > 0)) { // if all saved keys have been processed and a least one cache HIT
		if (payload+written+5 <= data_end) {
			payload[written++] = 'E';
			payload[written++] = 'N';
			payload[written++] = 'D';
			payload[written++] = '\r';
			payload[written++] = '\n';

			if (bpf_xdp_adjust_head(ctx, 0 - (int) (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr)
													+ sizeof(struct memcached_udp_header) + pctx->write_pkt_offset))) { // pop headers + previously written data
				return RX_OK;
			}

			void *data_end = (void *)(long)ctx->data_end;
			void *data = (void *)(long)ctx->data;
			struct iphdr *ip = data + sizeof(struct ethhdr);
			struct udphdr *udp = data + sizeof(struct ethhdr) + sizeof(*ip);
			payload = data + sizeof(struct ethhdr) + sizeof(*ip) + sizeof(*udp) + sizeof(struct memcached_udp_header);

			if (udp + 1 > data_end)
				return RX_OK;

			ip->tot_len = htons((payload+pctx->write_pkt_offset+written) - (char*)ip);
			ip->check = compute_ip_checksum(ip);
			udp->check = 0; // computing udp checksum is not required
			udp->len = htons((payload+pctx->write_pkt_offset+written) - (char*)udp);

			bpf_xdp_adjust_tail(ctx, 0 - (int) ((long) data_end - (long) (payload+pctx->write_pkt_offset+written))); // try to strip additional bytes
			
			NO_TEAR_INC(*cnt);
			return XDP_TX;
		}
	} else if (pctx->current_key == pctx->key_count) { // else if all saved keys have been processed but got no cache HIT; either because of a hash colision or a race with a cache update
		stats->hit_misprediction += pctx->key_count;
		bpf_xdp_adjust_head(ctx, ADJUST_HEAD_LEN - (int) (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct memcached_udp_header))); // pop to the old headers and transmit to netstack
		return RX_OK;
	} else if (pctx->current_key < BMC_MAX_KEY_IN_PACKET) { // else if there are still keys to process
		pctx->write_pkt_offset += written; // save packet write offset
		if (bpf_xdp_adjust_head(ctx, written)) // push written data
			return RX_DROP;
		NO_TEAR_INC(*cnt);
		call_ingress_program(ctx, BMC_PROG_XDP_WRITE_REPLY);
	}

	return RX_DROP;
}