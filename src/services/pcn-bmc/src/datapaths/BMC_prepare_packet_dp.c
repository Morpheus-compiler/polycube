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
BPF_TABLE("extern", int, uint64_t, pkt_cnt_prepare_packet, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	pcn_log(ctx, LOG_TRACE, "[Prepare Packet] Packet received");

	if (bpf_xdp_adjust_head(ctx, -ADJUST_HEAD_LEN)) // // pop empty packet buffer memory to increase the available packet size
		return RX_OK;

	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip);
	struct memcached_udp_header *memcached_udp_hdr = data + sizeof(*eth) + sizeof(*ip) + sizeof(*udp);
	char *payload = (char *) (memcached_udp_hdr + 1);
	void *old_data = data + ADJUST_HEAD_LEN;
	char *old_payload = (char *) (old_data + sizeof(*eth) + sizeof(*ip) + sizeof(*udp) + sizeof(*memcached_udp_hdr));

	if (payload >= data_end || old_payload+1 >= data_end)
		return RX_OK;

	// use old headers as a base; then update addresses and ports to create the new headers
	memmove(eth, old_data, sizeof(*eth) + sizeof(*ip) + sizeof(*udp) + sizeof(*memcached_udp_hdr));

	unsigned char tmp_mac[ETH_ALEN];
	__be32 tmp_ip;
	__be16 tmp_port;

	memcpy(tmp_mac, eth->h_source, ETH_ALEN);
	memcpy(eth->h_source, eth->h_dest, ETH_ALEN);
	memcpy(eth->h_dest, tmp_mac, ETH_ALEN);

	tmp_ip = ip->saddr;
	ip->saddr = ip->daddr;
	ip->daddr = tmp_ip;

	tmp_port = udp->source;
	udp->source = udp->dest;
	udp->dest = tmp_port;

	if (bpf_xdp_adjust_head(ctx, sizeof(*eth) + sizeof(*ip) + sizeof(*udp) + sizeof(*memcached_udp_hdr))) // push new headers
		return RX_OK;

	int zero = 0;
	uint64_t *cnt = pkt_cnt_prepare_packet.lookup(&zero);
	if (!cnt) {
		return RX_DROP;		
	}

	NO_TEAR_INC(*cnt);
	call_ingress_program(ctx, BMC_PROG_XDP_WRITE_REPLY);

	NO_TEAR_INC(*cnt);
	return RX_OK;
}