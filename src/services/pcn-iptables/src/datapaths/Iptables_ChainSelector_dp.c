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
     Chain Forarder
   ======================= */

struct elements {
  uint64_t bits[_MAXRULES];
};

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

enum {
  INPUT_LABELING,    // goto input chain and label packet
  FORWARD_LABELING,  // goto forward chain and label packet
  OUTPUT_LABELING,   // goto output chain and label packet
  PASS_LABELING,     // one chain is hit (IN/PUT/FWD) but there are no rules and
                     // default action is accept. Label packet and let it pass.
  PASS_NO_LABELING,  // OUTPUT chain is not hit, let the packet pass without
                     // labeling //NEVER HIT
  DROP_NO_LABELING   // one chain is hit (IN/PUT/FWD) but there are no rules and
                     // default action is DROP. //NEVER HIT
};

BPF_TABLE("extern", int, struct elements, sharedEle, 1);

BPF_TABLE("extern", int, struct packetHeaders, packet, 1);

#if _INGRESS_LOGIC
BPF_TABLE_SHARED("hash", __be32, int, localip, 256);

BPF_TABLE("extern", int, u64, pkts_default_Input, 1);
BPF_TABLE("extern", int, u64, bytes_default_Input, 1);

BPF_TABLE("extern", int, u64, pkts_default_Forward, 1);
BPF_TABLE("extern", int, u64, bytes_default_Forward, 1);

BPF_TABLE("extern", int, u64, default_action_Input, 1);
BPF_TABLE("extern", int, u64, default_action_Forward, 1);

BPF_TABLE("extern", int, u64, input_chain_empty, 1);
BPF_TABLE("extern", int, u64, forward_chain_empty, 1);
#endif

#if _EGRESS_LOGIC
BPF_TABLE("extern", __be32, int, localip, 256);

BPF_TABLE("extern", int, u64, pkts_default_Output, 1);
BPF_TABLE("extern", int, u64, bytes_default_Output, 1);

BPF_TABLE("extern", int, u64, default_action_Output, 1);

BPF_TABLE("extern", int, u64, output_chain_empty, 1);
#endif

// This is the percpu array containing the forwarding decision. ChainForwarder
// just lookup this value.
#if _INGRESS_LOGIC
BPF_TABLE_SHARED("percpu_array", int, int, forwardingDecision, 1);
#endif

#if _EGRESS_LOGIC
BPF_TABLE("extern", int, int, forwardingDecision, 1);
#endif

static __always_inline void updateForwardingDecision(int decision) {
  int key = 0;
  forwardingDecision.update(&key, &decision);
}

#if _INGRESS_LOGIC
static __always_inline void incrementDefaultCountersInput(u32 bytes) {
  u64 *value;
  int zero = 0;
  value = pkts_default_Input.lookup(&zero);
  if (value) {
    *value += 1;
  }

  value = bytes_default_Input.lookup(&zero);
  if (value) {
    *value += bytes;
  }
}

static __always_inline void incrementDefaultCountersForward(u32 bytes) {
  u64 *value;
  int zero = 0;
  value = pkts_default_Forward.lookup(&zero);
  if (value) {
    *value += 1;
  }

  value = bytes_default_Forward.lookup(&zero);
  if (value) {
    *value += bytes;
  }
}

static __always_inline int applyDefaultActionInput(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = default_action_Input.lookup(&zero);
  if (value && *value == 1) {
    //Default Action is ACCEPT
    updateForwardingDecision(PASS_LABELING);
    call_bpf_program(ctx, _CONNTRACK_LABEL_INGRESS);
    return RX_DROP;
  }

  return RX_DROP;
}

static __always_inline int applyDefaultActionForward(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = default_action_Forward.lookup(&zero);
  if (value && *value == 1) {
    //Default Action is ACCEPT
    updateForwardingDecision(PASS_LABELING);
    call_bpf_program(ctx, _CONNTRACK_LABEL_INGRESS);
    return RX_DROP;
  }

  return RX_DROP;
}

static __always_inline bool isInputChainEmpty(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = input_chain_empty.lookup(&zero);
  if (value && *value == 0) {
    return false;
  }

  return true;
}

static __always_inline bool isForwardChainEmpty(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = forward_chain_empty.lookup(&zero);
  if (value && *value == 0) {
    return false;
  }

  return true;
}
#endif

#if _EGRESS_LOGIC
static __always_inline void incrementDefaultCountersOutput(u32 bytes) {
  u64 *value;
  int zero = 0;
  value = pkts_default_Output.lookup(&zero);
  if (value) {
    *value += 1;
  }

  value = bytes_default_Output.lookup(&zero);
  if (value) {
    *value += bytes;
  }
}

static __always_inline int applyDefaultActionOutput(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = default_action_Output.lookup(&zero);
  if (value && *value == 1) {
    //Default Action is ACCEPT
    updateForwardingDecision(PASS_LABELING);
    call_bpf_program(ctx, _CONNTRACK_LABEL_EGRESS);
    return RX_DROP;
  }

  return RX_DROP;
}

static __always_inline bool isOutputChainEmpty(struct CTXTYPE *ctx) {
  u64 *value;

  int zero = 0;
  value = output_chain_empty.lookup(&zero);
  if (value && *value == 0) {
    return false;
  }

  return true;
}
#endif

static int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md) {
  pcn_log(ctx, LOG_DEBUG, "[ChainSelector] Receiving packet.");

// No rules in INPUT and FORWARD chain, and default action is accept
// let all the traffic to be labeled and pass.
  int key = 0;
  struct elements *result;

  struct packetHeaders *pkt;
  pkt = packet.lookup(&key);

  if (pkt == NULL) {
    // Not possible
    return RX_DROP;
  }

#if _INGRESS_LOGIC

  __be32 dstip = pkt->dstIp;
  goto FORWARD;
  __be32 *ip_matched = localip.lookup(&dstip);

  if (ip_matched) {
    pcn_log(ctx, LOG_DEBUG,
            "[ChainSelector] INGRESS. Dst ip matched. -->INPUT chain. ");
    goto INPUT;
  } else {
    pcn_log(ctx, LOG_DEBUG,
            "[ChainSelector] INGRESS. Dst ip NOT matched. -->FORWARD chain. ");
    goto FORWARD;
  }

INPUT:;
  if (isInputChainEmpty(ctx)) {
    pcn_log(
      ctx, LOG_DEBUG,
      "[ChainSelector] No INPUT chain instantiated. Apply default action for INPUT chain. ");
    // PASS_LABELING if ACCEPT
    // DROP_NO_LABELING if DROP
    incrementDefaultCountersInput(md->packet_len);
    return applyDefaultActionInput(ctx);
  }

  result = sharedEle.lookup(&key);
  if (result == NULL) {
    // Not possible
    return RX_DROP;
  }

  for (int i = 0; i < _NR_ELEMENTS_INPUT; ++i) {
    /*This is the first module, it initializes the percpu*/
    (result->bits)[i] = 0x7FFFFFFFFFFFFFFF;
  }

  // update map with INPUT_LABELING
  updateForwardingDecision(INPUT_LABELING);

  // call chain label INGRESS
  call_bpf_program(ctx, _CONNTRACK_LABEL_INGRESS);

  pcn_log(
      ctx, LOG_ERR,
      "[ChainSelector] Error while performing tail call to id _CONNTRACK_LABEL_INGRESS");
  return RX_DROP;

FORWARD:;
  if (isForwardChainEmpty(ctx)) {
    pcn_log(ctx, LOG_DEBUG,
          "[ChainSelector] No FORWARD chain instantiated. Apply default action for FORWARD "
          "chain. ");

    // PASS_LABELING if ACCEPT
    // DROP_NO_LABELING if DROP
    incrementDefaultCountersForward(md->packet_len);
    return applyDefaultActionForward(ctx);
  }
  result = sharedEle.lookup(&key);
  if (result == NULL) {
    // Not possible
    return RX_DROP;
  }

  for (int i = 0; i < _NR_ELEMENTS_FORWARD; ++i) {
    /*This is the first module, it initializes the percpu*/
    (result->bits)[i] = 0x7FFFFFFFFFFFFFFF;
  }

  // update map with FORWARD_LABELING
  updateForwardingDecision(FORWARD_LABELING);

  // call chain label INGRESS
  call_bpf_program(ctx, _CONNTRACK_LABEL_INGRESS);

  pcn_log(
      ctx, LOG_ERR,
      "[ChainSelector] Error while performing tail call to id _CONNTRACK_LABEL_INGRESS");
  return RX_DROP;

#endif

#if _EGRESS_LOGIC

  __be32 srcip = pkt->srcIp;
  __be32 *ip_matched = localip.lookup(&srcip);

  // __be32* ip_matched = localip.lookup(&ip->saddr);
  if (ip_matched) {
    pcn_log(ctx, LOG_DEBUG,
            "[ChainSelector] EGRESS. Src ip matched. -->OUTPUT chain. ");
    goto OUTPUT;
  } else {
    pcn_log(ctx, LOG_DEBUG,
            "[ChainSelector] EGRESS. Src ip NOT matched. -->PASS. ");
    goto PASS;
  }

OUTPUT:;
  if (isOutputChainEmpty(ctx)) {
    pcn_log(
      ctx, LOG_DEBUG,
      "[ChainSelector] No OUTPUT chain instantiated. Apply default action for OUTPUT chain. ");

    // PASS_LABELING if ACCEPT
    // DROP_NO_LABELING if DROP
    incrementDefaultCountersOutput(md->packet_len);
    return applyDefaultActionOutput(ctx);
  }
  result = sharedEle.lookup(&key);
  if (result == NULL) {
    // Not possible
    return RX_DROP;
  }

  for (int i = 0; i < _NR_ELEMENTS_OUTPUT; ++i) {
    /*This is the first module, it initializes the percpu*/
    (result->bits)[i] = 0x7FFFFFFFFFFFFFFF;
  }

  // update map with OUTPUT_LABELING
  updateForwardingDecision(OUTPUT_LABELING);

  // call chain label INGRESS
  call_bpf_program(ctx, _CONNTRACK_LABEL_EGRESS);
  pcn_log(
      ctx, LOG_ERR,
      "[ChainSelector] Error while performing tail call to id _CONNTRACK_LABEL_EGRESS");
  return RX_DROP;

PASS:;
  pcn_log(ctx, LOG_DEBUG,
          "[ChainSelector] No hit for OUTPUT chain. Let the packet PASS_NO_LABELING. ");
  return RX_OK;

#endif

  return RX_DROP;
}