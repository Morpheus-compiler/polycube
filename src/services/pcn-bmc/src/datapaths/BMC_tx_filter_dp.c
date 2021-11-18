#include <uapi/linux/bpf.h>
#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/udp.h>
#include <uapi/linux/tcp.h>

#include "BMC_common.h"

BPF_TABLE("extern", int, struct bmc_stats, map_stats, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data     = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip);
	char *payload = data + sizeof(*eth) + sizeof(*ip) + sizeof(*udp) + sizeof(struct memcached_udp_header);
	unsigned int zero = 0;

	long len = (long)ctx->data_end - (long)ctx->data;

	pcn_log(ctx, LOG_TRACE, "[TX filter] Packet received");

	// if the size exceeds the size of a cache entry do not bother going further
	if (len > BMC_MAX_CACHE_DATA_SIZE + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct memcached_udp_header))
		return RX_OK;

	if (ip + 1 > data_end)
		return RX_OK;

	if (ip->protocol != IPPROTO_UDP)
		return RX_OK;

	if (udp + 1 > data_end)
		return RX_OK;

	__be16 sport = udp->source;

	if (sport == htons(11211) && payload+5+1 <= data_end && payload[0] == 'V' && payload[1] == 'A' && payload[2] == 'L'
		&& payload[3] == 'U' && payload[4] == 'E' && payload[5] == ' ') { // if this is a GET reply

		struct bmc_stats *stats = map_stats.lookup(&zero);
		if (!stats) {
			return XDP_PASS;
		}
		stats->get_resp_count++;

		call_egress_program(ctx, BMC_PROG_TC_UPDATE_CACHE);
	}

	return TC_ACT_OK;
}