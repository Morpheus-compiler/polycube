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
BPF_TABLE("extern", int, uint64_t, pkt_cnt_invalidate_cache, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip);
	char *payload = (char *) (tcp + 1);
	unsigned int zero = 0;

	pcn_log(ctx, LOG_TRACE, "[Invalidate Cache] Packet received");

	if (payload >= data_end)
		return RX_OK;

	struct bmc_stats *stats = map_stats.lookup(&zero);
	if (!stats) {
		return RX_OK;
	}

	uint64_t *cnt = pkt_cnt_invalidate_cache.lookup(&zero);
	if (!cnt) {
		return RX_DROP;		
	}

	u32 hash;
	int set_found = 0, key_found = 0;

#pragma clang loop unroll(disable)
	for (unsigned int off = 0; off < BMC_MAX_PACKET_LENGTH && payload+off+1 <= data_end; off++) {

		if (set_found == 0 && payload[off] == 's' && payload+off+3 <= data_end && payload[off+1] == 'e' && payload[off+2] == 't') {
			set_found = 1;
			off += 3; // move offset after the set keywork, at the next iteration 'off' will either point to a space or the start of the key
			stats->set_recv_count++;
		}
		else if (key_found == 0 && set_found == 1 && payload[off] != ' ') {
			if (payload[off] == '\r') { // end of packet
				set_found = 0;
				key_found = 0;
			} else { // found the start of the key
				hash = FNV_OFFSET_BASIS_32;
				hash ^= payload[off];
				hash *= FNV_PRIME_32;
				key_found = 1;
			}
		}
		else if (key_found == 1) {
			if (payload[off] == ' ') { // found the end of the key
				u32 cache_idx = hash % BMC_CACHE_ENTRY_COUNT;
				struct bmc_cache_entry *entry = map_kcache.lookup(&cache_idx);
				if (!entry) {
					return RX_OK;
				}
				//bpf_spin_lock(&entry->lock);
				//if (entry->valid) {
					// entry->valid = 0;
				// __sync_val_compare_and_swap(&entry->valid, 1, 0);
				if (entry->valid) {
					struct bmc_cache_entry new_entry = {};
					__builtin_memcpy(&new_entry, entry, sizeof(struct bmc_cache_entry));
					new_entry.valid = 0;

					map_kcache.update(&cache_idx, &new_entry);
					stats->invalidation_count++;
				}
				// char expected = 1;
				// if (_atomic_compare_exchange_n(&entry->valid, &expected, 0, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
				// 	stats->invalidation_count++;
				// }

				//bpf_spin_unlock(&entry->lock);
				set_found = 0;
				key_found = 0;
			}
			else { // still processing the key
				hash ^= payload[off];
				hash *= FNV_PRIME_32;
			}
		}
	}
	

	NO_TEAR_INC(*cnt);
	return RX_OK;
}