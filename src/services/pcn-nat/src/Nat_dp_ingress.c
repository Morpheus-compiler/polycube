#define NATTYPE NATTYPE_INGRESS
// session table also used by egrees programs
BPF_TABLE_SHARED("hash", struct st_k, struct st_v, egress_session_table,
                 NAT_MAP_DIM);
BPF_TABLE_SHARED("hash", struct st_k, struct st_v, ingress_session_table,
                 NAT_MAP_DIM);
// only needed in ingress
// DNAT + PORTFORWARDING rules
struct dp_k {
  u32 mask;
  __be32 external_ip;
  __be16 external_port;
  uint8_t proto;
};
struct dp_v {
  __be32 internal_ip;
  __be16 internal_port;
  uint8_t entry_type;
};
BPF_LPM_TRIE(dp_rules, struct dp_k, struct dp_v, 1024);