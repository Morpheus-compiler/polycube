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
BPF_TABLE("extern", int, uint64_t, pkt_cnt_update_cache, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	char *payload = (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct memcached_udp_header));
	unsigned int zero = 0;

	pcn_log(ctx, LOG_TRACE, "[Update Cache] Packet received");

	u32 hash = FNV_OFFSET_BASIS_32;

	// compute the key hash
#pragma clang loop unroll(disable)
	for (unsigned int off = 6; off-6 < BMC_MAX_KEY_LENGTH && payload+off+1 <= data_end && payload[off] != ' '; off++) {
		hash ^= payload[off];
		hash *= FNV_PRIME_32;
	}

	u32 cache_idx = hash % BMC_CACHE_ENTRY_COUNT;
	struct bmc_cache_entry *entry = map_kcache.lookup(&cache_idx);
	if (!entry) {
		return RX_OK;
	}

	//bpf_spin_lock(&entry->lock);
	if (entry->valid && entry->hash == hash) { // cache is up-to-date; no need to update
		int diff = 0;
		// loop until both bytes are spaces ; or break if they are different
#pragma clang loop unroll(disable)
		for (unsigned int off = 6; off-6 < BMC_MAX_KEY_LENGTH && payload+off+1 <= data_end && off < entry->len && (payload[off] != ' ' || entry->data[off] != ' '); off++) {
			if (entry->data[off] != payload[off]) {
				diff = 1;
				break;
			}
		}
		if (diff == 0) {
			//bpf_spin_unlock(&entry->lock);
			return RX_OK;
		}
	}

	struct bmc_cache_entry new_entry = {};
	entry = &new_entry;
	__builtin_memcpy(&new_entry, entry, sizeof(struct bmc_cache_entry));

	unsigned int count = 0;
	entry->len = 0;
	// store the reply from start to the '\n' that follows the data
#pragma clang loop unroll(disable)
	for (unsigned int j = 0; j < BMC_MAX_CACHE_DATA_SIZE && payload+j+1 <= data_end && count < 2; j++) {
		entry->data[j] = payload[j];
		entry->len++;
		if (payload[j] == '\n')
			count++;
	}

	if (count == 2) { // copy OK
		entry->valid = 1;
		entry->hash = hash;
		//bpf_spin_unlock(&entry->lock);
		pcn_log(ctx, LOG_TRACE, "[Update Cache] Update entry with at index: %u with hash: %u", cache_idx, hash);
		struct bmc_stats *stats = map_stats.lookup(&zero);
		if (!stats) {
			return RX_OK;
		}
		stats->update_count++;
	} else {
		//bpf_spin_unlock(&entry->lock);
	}

	uint64_t *cnt = pkt_cnt_update_cache.lookup(&zero);
	if (!cnt) {
		return RX_DROP;		
	}

	NO_TEAR_INC(*cnt);
	map_kcache.update(&cache_idx, &new_entry);

	return RX_OK;
}