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
   Match on TCP Flags.
   ======================= */

// _DIRECTION = {INPUT, FORWARD, OUTPUT}
// _TYPE = {src, dst}

// Same as others
// btw, no wildcard since all possible combinations already present

// #include <bcc/helpers.h>
// #include <uapi/linux/in.h>

#define IPPROTO_TCP 6

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

BPF_TABLE("extern", int, struct packetHeaders, packet, 1);
static __always_inline struct packetHeaders *getPacket() {
  int key = 0;
  return packet.lookup(&key);
}

#if _NR_ELEMENTS > 0
struct elements {
  uint64_t bits[_MAXRULES];
};

BPF_TABLE_RO("array", int, struct elements, tcpFlags_DIRECTION, 256, 1);
//BPF_ARRAY(tcpFlags_DIRECTION, struct elements, 256);
static __always_inline struct elements *getBitVect(int *key) {
  return tcpFlags_DIRECTION.lookup(key);
}

BPF_TABLE("extern", int, struct elements, sharedEle, 1);
static __always_inline struct elements *getShared() {
  int key = 0;
  return sharedEle.lookup(&key);
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
/*The struct elements and the lookup table are defined only if _NR_ELEMENTS>0,
 * so this code has to be used only in this case.*/
  int key = 0;
  struct packetHeaders *pkt = getPacket();
  if (pkt == NULL) {
    // Not possible
    return RX_DROP;
  }
  if (pkt->l4proto != IPPROTO_TCP) {
    pcn_log(ctx, LOG_DEBUG, "[TCPFlagsLookup] Code flags _DIRECTION ignoring packet. ");
    call_bpf_program(ctx, _NEXT_HOP_1);
    return RX_DROP;
  }
  pcn_log(ctx, LOG_DEBUG, "[TCPFlagsLookup] Code flags _DIRECTION receiving packet. ");

  int flags = 0;
  flags = pkt->flags;

  struct elements *ele = getBitVect(&flags);

  if (ele == NULL) {
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
        (result->bits)[i] = (result->bits)[i] & (ele->bits)[i];

        if (result->bits[i])
          isAllZero = false;
      }

      if (isAllZero) {
        pcn_log(ctx, LOG_DEBUG,
                "[TCPFlagsLookup] Bitvector is all zero. Break pipeline for TcpFlags_DIRECTION");
        incrementDefaultCounters_DIRECTION(md->packet_len);
        return applyDefaultAction(ctx);
      }
    }  // if result == NULL
  }    // if ele==NULL

  call_bpf_program(ctx, _NEXT_HOP_1);

  return RX_DROP;
}