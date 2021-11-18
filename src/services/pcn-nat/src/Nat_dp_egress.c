#define NATTYPE NATTYPE_EGRESS
// defined in ingress program
BPF_TABLE("extern", struct st_k, struct st_v, egress_session_table,
          NAT_MAP_DIM);
BPF_TABLE("extern", struct st_k, struct st_v, ingress_session_table,
          NAT_MAP_DIM);
// only needed in egress
// SNAT + MASQUERADE rules
struct sm_k {
  u32 internal_netmask_len;
  __be32 internal_ip;
};
struct sm_v {
  __be32 external_ip;
  uint8_t entry_type;
};

struct free_port_v {
  u16 port;
  struct bpf_spin_lock lock;
};

BPF_LPM_TRIE(sm_rules, struct sm_k, struct sm_v, 1024);
// Port numbers
BPF_TABLE("array", u32, struct free_port_v, first_free_port, 1);
static inline __be16 get_free_port() {
  u32 i = 0;
  struct free_port_v *new_port_ = first_free_port.lookup(&i);
  if (!new_port_)
    return 0;
  bpf_spin_lock(&new_port_->lock);
  if (new_port_->port < 1024 || new_port_->port == 65535)
    new_port_->port = 1024;
  new_port_->port = new_port_->port + 1;
  
  u16 port = new_port_->port;
  bpf_spin_unlock(&new_port_->lock);

  return bpf_htons(port);
}