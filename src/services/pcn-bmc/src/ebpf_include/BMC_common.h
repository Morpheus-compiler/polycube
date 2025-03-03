#ifndef __BMC_COMMON_DP_H
#define __BMC_COMMON_DP_H

#include <uapi/linux/bpf.h>
#include <uapi/linux/in.h>
#include <stddef.h>

#ifndef memmove
# define memmove(dest, src, n)  __builtin_memmove((dest), (src), (n))
#endif

#define BMC_MAX_KEY_LENGTH 40
#define BMC_MAX_VAL_LENGTH 40
#define BMC_MAX_ADDITIONAL_PAYLOAD_BYTES 54
#define BMC_MAX_CACHE_DATA_SIZE BMC_MAX_KEY_LENGTH+BMC_MAX_VAL_LENGTH+BMC_MAX_ADDITIONAL_PAYLOAD_BYTES
#define BMC_MAX_KEY_IN_MULTIGET 30
// #define BMC_CACHE_ENTRY_COUNT 3250000
#define BMC_CACHE_ENTRY_COUNT 50000
#define BMC_MAX_PACKET_LENGTH 1500
#define BMC_MAX_KEY_IN_PACKET BMC_MAX_KEY_IN_MULTIGET

#define FNV_OFFSET_BASIS_32		2166136261
#define FNV_PRIME_32			16777619

#define ADJUST_HEAD_LEN 128

enum {
	BMC_PROG_XDP_RX_FILTER = 0,
	BMC_PROG_XDP_HASH_KEYS,
	BMC_PROG_XDP_PREPARE_PACKET,
	BMC_PROG_XDP_WRITE_REPLY,
	BMC_PROG_XDP_INVALIDATE_CACHE
};

enum {
	BMC_PROG_TC_TX_FILTER = 0,
	BMC_PROG_TC_UPDATE_CACHE
};

struct memcached_udp_header {
    __be16 request_id;
    __be16 seq_num;
    __be16 num_dgram;
    __be16 unused;
    char data[];
} __attribute__((__packed__));

struct bmc_cache_entry {
	// struct bpf_spin_lock lock;
	uint32_t len;
	char valid;
	int hash;
	char data[BMC_MAX_CACHE_DATA_SIZE];
};

struct bmc_stats {
	unsigned int get_recv_count;			// Number of GET command received
	unsigned int set_recv_count;			// Number of SET command received
	unsigned int get_resp_count;			// Number of GET command reply analyzed
	unsigned int hit_misprediction;			// Number of keys that were expected to hit but did not (either because of a hash colision or a race with an invalidation/update)
	unsigned int hit_count;				// Number of HIT in kernel cache
	unsigned int miss_count;			// Number of MISS in kernel cache
	unsigned int update_count;			// Number of kernel cache updates
	unsigned int invalidation_count;		// Number of kernel cache entry invalidated
};

/* keys */
struct memcached_key {
	u32 hash;
	char data[BMC_MAX_KEY_LENGTH];
	unsigned int len;
};

struct parsing_context {
	unsigned int key_count;
	unsigned int current_key;
	unsigned short read_pkt_offset;
	unsigned short write_pkt_offset;
};

#endif // of __BMC_COMMON_DP_H