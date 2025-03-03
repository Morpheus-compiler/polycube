/*
 * Copyright 2017 The Polycube Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* =======================
   Match on IP Source
   ======================= */

// _DIRECTION = {INPUT, FORWARD, OUTPUT}
// _TYPE = {src, dst}

// #include <bcc/helpers.h>

struct packetHeaders {
  uint32_t srcIp;
  uint32_t dstIp;
  uint8_t l4proto;
  uint16_t srcPort;
  uint16_t dstPort;
  uint8_t flags;
  uint32_t seqN;
  uint32_t ackN;
  uint8_t connStatus;
};

#if _NR_ELEMENTS > 0
struct elements {
  uint64_t bits[_MAXRULES];
};

struct lpm_k {
  u32 netmask_len;
  __be32 ip;
};

BPF_TABLE("extern", int, struct elements, sharedEle, 1);
static __always_inline struct elements *getShared() {
  int key = 0;
  return sharedEle.lookup(&key);
}

BPF_LPM_TRIE(ip_TYPETrie_DIRECTION, struct lpm_k, struct elements, 1024);

static __always_inline struct elements *getBitVect(struct lpm_k *key) {
  return ip_TYPETrie_DIRECTION.lookup(key);
}
#endif

BPF_TABLE("extern", int, u64, pkts_default__DIRECTION, 1);
BPF_TABLE("extern", int, u64, bytes_default__DIRECTION, 1);
BPF_TABLE("extern", int, u64, default_action__DIRECTION, 1);

static __always_inline int applyDefaultAction(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = default_action__DIRECTION.lookup(&zero);
  if (value && *value == 1) {
    //Default Action is ACCEPT
    call_bpf_program(ctx, _CONNTRACKTABLEUPDATE);
    return RX_DROP;
  }

  return RX_DROP;
}

static __always_inline void incrementDefaultCounters_DIRECTION(u32 bytes) {
  u64 *value;
  int zero = 0;
  value = pkts_default__DIRECTION.lookup(&zero);
  if (value) {
    *value += 1;
  }

  value = bytes_default__DIRECTION.lookup(&zero);
  if (value) {
    *value += bytes;
  }
}

BPF_TABLE("extern", int, struct packetHeaders, packet, 1);
static __always_inline struct packetHeaders *getPacket() {
  int key = 0;
  return packet.lookup(&key);
}

static int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md) {
  pcn_log(ctx, LOG_DEBUG, "[Ip_TYPELookup] Code Ip_TYPE_DIRECTION receiving packet. ");

/*The struct elements and the lookup table are defined only if NR_ELEMENTS>0, so
 * this code has to be used only in those cases.*/
#if _NR_ELEMENTS > 0
  int key = 0;
  struct packetHeaders *pkt = getPacket();
  if (pkt == NULL) {
    // Not possible
    return RX_DROP;
  }

  struct lpm_k lpm_key = {32, pkt->_TYPEIp};
  struct elements *ele = getBitVect(&lpm_key);

//  u32 result = jhash(&lpm_key, sizeof(lpm_key), JHASH_INITVAL);
//  if(!result)
//    return RX_DROP;
//
//  struct elements ele_try;
//
//  for (int i = 0; i < _NR_ELEMENTS; ++i) {
//    /*This is the first module, it initializes the percpu*/
//    (ele_try.bits)[i] = 0x7FFFFFFFFFFFFFFF;
//  }
//
//  struct elements *ele = &ele_try;

  if (ele == NULL) {
    pcn_log(ctx, LOG_DEBUG, "[Ip_TYPELookup] No match. (pkt->_TYPEIp: %u) ", pkt->_TYPEIp);
    incrementDefaultCounters_DIRECTION(md->packet_len);
    return applyDefaultAction(ctx);
  } else {
    struct elements *result = getShared();
    if (result == NULL) {
      /*Can't happen. The PERCPU is preallocated.*/
      return RX_DROP;
    } else {
      bool isAllZero = true;

      for (int i = 0; i < _NR_ELEMENTS; ++i) {
        /*This is the first module, it initializes the percpu*/
        (result->bits)[i] = (result->bits)[i] & (ele->bits)[i];

        if (result->bits[i] != 0)
          isAllZero = false;
      }

      if (isAllZero) {
        pcn_log(ctx, LOG_DEBUG,
                "[Ip_TYPELookup] Bitvector is all zero. Break pipeline for Ip_TYPE_DIRECTION");
        incrementDefaultCounters_DIRECTION(md->packet_len);
        return applyDefaultAction(ctx);
      }
    }  // if result == NULL
  }    // if ele==NULL
  call_bpf_program(ctx, _NEXT_HOP_1);

#else
  return RX_DROP;
#endif

  return RX_DROP;
}