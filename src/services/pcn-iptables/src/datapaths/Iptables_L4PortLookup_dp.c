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
   Match on Transport Destination Port.
   ======================= */

// _DIRECTION = {INPUT, FORWARD, OUTPUT}
// _TYPE = {src, dst}

// #include <bcc/helpers.h>
// #include <uapi/linux/in.h>
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

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

// PERCPU ARRAY
// with parsed headers for current packet
BPF_TABLE("extern", int, struct packetHeaders, packet, 1);
static __always_inline struct packetHeaders *getPacket() {
  int key = 0;
  return packet.lookup(&key);
}

#if _NR_ELEMENTS > 0
struct elements {
  uint64_t bits[_MAXRULES];
};

BPF_TABLE("extern", int, struct elements, sharedEle, 1);
static __always_inline struct elements *getShared() {
  int key = 0;
  return sharedEle.lookup(&key);
}

BPF_TABLE_RO("hash", uint16_t, struct elements, _TYPEPorts_DIRECTION, 65535, 1);
//BPF_HASH(_TYPEPorts_DIRECTION, uint16_t, struct elements);
static __always_inline struct elements *getBitVect(uint16_t *key) {
  return _TYPEPorts_DIRECTION.lookup(key);
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

static int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md) {
  /*
   * The struct elements and the lookup table are defined only if _NR_ELEMENTS>0,
   * so this code has to be used only in this case.
   */
  int key = 0;
  struct packetHeaders *pkt = getPacket();
  if (pkt == NULL) {
    // Not possible
    return RX_DROP;
  }

  uint16_t _TYPEPort = 0;
  if (pkt->l4proto != IPPROTO_TCP && pkt->l4proto != IPPROTO_UDP) {
    pcn_log(ctx, LOG_DEBUG, "[L4_TYPEPortLookup] Code _TYPEPort _DIRECTION ignoring packet. ");
    call_bpf_program(ctx, _NEXT_HOP_1);
    return RX_DROP;
  } else {
    pcn_log(ctx, LOG_DEBUG, "[L4_TYPEPortLookup] Code _TYPEPort _DIRECTION receiving packet. ");
    // var containing the port, locally stored
    _TYPEPort = pkt->_TYPEPort;
  }

  // Ports are stored in an hashmap
  // A. map[0] (if exists) contains wildcard match
  // B. map[port] (if some exists) contain ports matching

  // if no match in A. and B.
  // we assume current bitvector is 0x000000...
  // also AND returns 0x0000...
  // so we can apply DEFAULT action with no additional cost.

  struct elements *result = getShared();

  bool isAllZero = true;
  if (result == NULL) {
    /*Can't happen. The PERCPU is preallocated.*/
    return RX_DROP;
  } 
  
  struct elements *ele = getBitVect(&_TYPEPort);

  if (ele == NULL) {
    // if lookup with port fails, we have to
    // a. verify if we have a wildcard key (0)
    // b. if so, use to bitvector from wildcard key
    _TYPEPort = 0;
    pcn_log(ctx, LOG_DEBUG, "[L4_TYPEPortLookup] +WILDCARD RULE+");
    ele = getBitVect(&_TYPEPort);
    if (ele == NULL) {
      pcn_log(ctx, LOG_DEBUG, "[L4_TYPEPortLookup] No match. ");
      incrementDefaultCounters_DIRECTION(md->packet_len);
      return applyDefaultAction(ctx);
    }
  }

  int i = 0;
  for (i = 0; i < _NR_ELEMENTS; ++i) {
    (result->bits)[i] = (result->bits)[i] & (ele->bits)[i];
    if (result->bits[i] != 0)
      isAllZero = false;
  }
  goto NEXT;

NEXT:;
  if (isAllZero) {
    pcn_log(ctx, LOG_DEBUG,
            "[L4_TYPEPortLookup] Bitvector is all zero. Break pipeline for _TYPEPort _DIRECTION");
    incrementDefaultCounters_DIRECTION(md->packet_len);
    return applyDefaultAction(ctx);
  }
  call_bpf_program(ctx, _NEXT_HOP_1);

  return RX_DROP;
}