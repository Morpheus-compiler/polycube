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
      First Bit Search
   ======================= */

// #include <bcc/helpers.h>
BPF_TABLE("array", int, uint16_t, index64, 64);
//BPF_ARRAY(index64, uint16_t, 64);

// This Struct is initialized by the parser
// This is allocated with MAX Number of possible elements.
// _NR_ELEMENTS is the current value
struct elements {
  // _MAXRULES defined by control plane as MAXRULES/63 + .....
  uint64_t bits[_MAXRULES];
};

// PERCPU ARRAY containing current bitvector (already ANDed in previous modules)
BPF_TABLE("extern", int, struct elements, sharedEle, 1);
static __always_inline struct elements *getShared() {
  int key = 0;
  return sharedEle.lookup(&key);
}

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
// since pragma unroll is not working with N=1
// this case is managed as separated case

int key = 0;
struct elements *ele = getShared();
if (ele == NULL) {
  /*Can't happen. The PERCPU is preallocated.*/
  return RX_DROP;
}
uint16_t *matchingResult = 0;

#pragma nounroll
  for (int i = 0; i < _NR_ELEMENTS; ++i) {
    uint64_t bits = (ele->bits)[i];
    if (bits != 0) {
      int index = (int)(((bits ^ (bits - 1)) * 0x03f79d71b4cb0a89) >> 58);
      matchingResult = index64.lookup(&index);
      if (matchingResult == NULL) {
        /*This can't happen*/
        return RX_DROP;
      }
      index64.update(&index, matchingResult);

      int globalBit = *matchingResult + i * 63;
      pcn_log(ctx, LOG_DEBUG,
              "[BitScan]  _DIRECTION Matching element %d offset %d. ", i,
              *matchingResult);
      (ele->bits)[0] = globalBit;
      call_bpf_program(ctx, _NEXT_HOP_1);

    }  // ele->bits[i] != 0
  }    // end loop

  // DEFAULT ACTION (?)
  pcn_log(ctx, LOG_DEBUG, "[BitScan] No bit set to 1. ");
  incrementDefaultCounters_DIRECTION(md->packet_len);
  return applyDefaultAction(ctx);
}
