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
BPF_TABLE("extern", int, uint64_t, pkt_cnt_hash_keys, 1);

static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	char *payload = (char *) data;
	unsigned int zero = 0;

	pcn_log(ctx, LOG_TRACE, "[Hash Keys] Packet received");
	// bpf_trace_printk("[Hash Keys] Packet received");

	if (payload >= data_end)
		return RX_OK;

	struct parsing_context *pctx = map_parsing_context.lookup(&zero);
	if (!pctx) {
		return RX_OK;
	}

	struct memcached_key *key = map_keys.lookup(&pctx->key_count);
	if (!key) {
		return RX_OK;
	}
	key->hash = FNV_OFFSET_BASIS_32;

	unsigned int off, done_parsing = 0, key_len = 0;

	// compute the key hash
#pragma clang loop unroll(disable)
	for (off = 0; off < BMC_MAX_KEY_LENGTH+1; off++) {
		if (payload+off+1 > data_end) {
			break;
		}

		if (payload[off] == '\r') {
			done_parsing = 1;
			break;
		}
		else if (payload[off] == ' ') {
			break;
		}
		else if (payload[off] != ' ') {
			key->hash ^= payload[off];
			key->hash *= FNV_PRIME_32;
			key_len++;
		}
	}

	if (key_len == 0 || key_len > BMC_MAX_KEY_LENGTH) {
		bpf_xdp_adjust_head(ctx, 0 - (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct memcached_udp_header) + pctx->read_pkt_offset)); // unexpected key, let the netstack handle it
		return RX_OK;
	}

	u32 cache_idx = key->hash % BMC_CACHE_ENTRY_COUNT;

	pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Hash key is %u, cache index is %u", key->hash, cache_idx);

	// struct bmc_cache_entry *entry;
	// struct bmc_cache_entry tmp_entry;

	// if (cache_idx == 0x2c65) {
	// 	pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Hitting cached entry");
	// 	tmp_entry.len = 0x3f;
	// 	tmp_entry.valid = 0x1;
	// 	tmp_entry.hash = 0xae9fd905;
	// 	char val[] = "VALUE 0000000000000000 0 32\r\n, sagittis\net eleifend rutrum, c\r\n";
	// 	__builtin_memcpy(tmp_entry.data, val, sizeof(val)/sizeof(val[0]));
	// 	entry = &tmp_entry;
	// } else {
	// 	entry = map_kcache.lookup(&cache_idx);
	// 	if (!entry) { // should never happen since cache map is of type BPF_MAP_TYPE_ARRAY
	// 		return RX_OK;
	// 	}
	// }

	struct bmc_cache_entry *entry = map_kcache.lookup(&cache_idx);
	if (!entry) { // should never happen since cache map is of type BPF_MAP_TYPE_ARRAY
		return RX_OK;
	}

	// pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry len is: %u", entry->len);
	// pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry valid is: %u", entry->valid);
	// pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry hash is: %u", entry->hash);
	// for (int i = 0; i < 40; i++) {
	// 	if (i < BMC_MAX_CACHE_DATA_SIZE && i < entry->len)
	// 		// bpf_trace_printk("[Hash Keys] Entry data is: %u", entry->data[i]);
	// 		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry data is: %u", entry->data[i]);
	// }

	// struct bmc_cache_entry tmp_entry = {};
	// __builtin_memcpy(tmp_entry.data, "prova", 5);
	// tmp_entry.hash = entry->hash;
	// if (tmp_entry.hash == 0x1234) {
	// 	map_kcache.update(&cache_idx, &tmp_entry);
	// 	return RX_DROP;
	// }
	if (entry->valid) {
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry is valid");
	} else {
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry is NOT valid");
	}

	if (entry->hash == key->hash) {
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry hash and key hash are equals");
	} else {
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Entry hash and key hash are different: %u, %u", entry->hash, key->hash);
	}

	//bpf_spin_lock(&entry->lock);
	if (entry->valid && entry->hash == key->hash) { // potential cache hit
		//bpf_spin_unlock(&entry->lock);
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] potential Cache Hit");
		unsigned int i = 0;	
	 // for (; i < key_len && payload+i+1 <= data_end; i++) { // copy the request key to compare it with the one stored in the cache later
	 // 	key->data[i] = payload[i];
	 // }
#pragma clang loop unroll(disable)	
		for (; i < BMC_MAX_KEY_LENGTH; i++) { // copy the request key to compare it with the one stored in the cache later
			if (i < key_len && payload+i+1 <= data_end) {
				key->data[i] = payload[i];
			} else { break; }
		}
		// key->data[i] = payload[i];
		key->len = key_len;
		pctx->key_count++;
	} else { // cache miss
		//bpf_spin_unlock(&entry->lock);
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Cache Miss");
		struct bmc_stats *stats = map_stats.lookup(&zero);
		if (!stats) {
			return RX_OK;
		}
		stats->miss_count++;
	}

	uint64_t *cnt = pkt_cnt_hash_keys.lookup(&zero);
	if (!cnt) {
		return RX_DROP;		
	}
	

	if (done_parsing) { // the end of the request has been reached
		bpf_xdp_adjust_head(ctx, 0 - (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct memcached_udp_header) + pctx->read_pkt_offset)); // pop headers + 'get ' + previous keys
		if (pctx->key_count > 0) {
			pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Calling Prepare Packet");
			NO_TEAR_INC(*cnt);
			return RX_DROP;
			call_ingress_program(ctx, BMC_PROG_XDP_PREPARE_PACKET);
		}
	} else { // more keys to process
		off++; // move offset to the start of the next key
		pctx->read_pkt_offset += off;
		if (bpf_xdp_adjust_head(ctx, off)) // push the previous key
			return RX_OK;
		pcn_log(ctx, LOG_DEBUG, "[Hash Keys] Calling Hash Key again!");
		NO_TEAR_INC(*cnt);
		call_ingress_program(ctx, BMC_PROG_XDP_HASH_KEYS);
	}

	NO_TEAR_INC(*cnt);
	return RX_OK;
}