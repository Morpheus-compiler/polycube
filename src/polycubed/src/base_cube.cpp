/*
 * Copyright 2018 The Polycube Authors
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

#include "base_cube.h"
#include "bpf_module.h"
#include "polycube/common.h"

namespace polycube {
namespace polycubed {

/** Function to compute a base Unix Epoch timestamp to be used as reference by
 * all the timestamp computed using pcn_get_packet_epoch(ctx).
 *
 * Supposing the packet arrives at time X and we precomputed Unix Epoch and
 * kernel timer at time Y:
 *
 * Epoch_at_X = Epoch_at_Y + (Kernel_at_X - Kernel_at_Y)
 *
 * where Kernel_at_X is given by the function bpf_ktime_get_ns()
 * */
uint64_t genBaseTime() {
  // getting system clock (to retrieve Unix Epoch)
  auto epoch = std::chrono::system_clock::now();
  // getting the same time as bpf_ktime_get_ns()
  auto up = std::chrono::steady_clock::now();
  // return value to be summed to packet's bpf_ktime_get_ns()
  return epoch.time_since_epoch().count() - up.time_since_epoch().count();
}

std::vector<std::string> BaseCube::default_cflags_ = {
    std::string("-D_POLYCUBE_MAX_NODES=") +
        std::to_string(PatchPanel::_POLYCUBE_MAX_NODES),
    std::string("-D_POLYCUBE_MAX_BPF_PROGRAMS=") +
        std::to_string(_POLYCUBE_MAX_BPF_PROGRAMS),
    std::string("-D_POLYCUBE_MAX_PORTS=") + std::to_string(_POLYCUBE_MAX_PORTS),
    std::string("-D_EPOCH_BASE=") + std::to_string(genBaseTime()),
};

BaseCube::BaseCube(const std::string &name, const std::string &service_name,
                   const std::string &master_code, PatchPanel &patch_panel,
                   LogLevel level, CubeType type, bool dyn_opt_enabled, const std::vector<std::string> &cflags)
    : name_(name),
      service_name_(service_name),
      logger(spdlog::get("polycubed")),
      uuid_(GuidGenerator().newGuid()),
      patch_panel_(patch_panel),
      ingress_index_(0),
      egress_index_(0),
      level_(level),
      type_(type),
      dyn_opt_enabled_(dyn_opt_enabled),
      id_(id_generator_.acquire()) {

  cflags_.insert(cflags_.end(), default_cflags_.begin(), default_cflags_.end());
  cflags_.insert(cflags_.end(), cflags.begin(), cflags.end());

  std::lock_guard<std::mutex> guard(bcc_mutex);

  // create master program that contains some maps definitions
  master_program_ =
      std::unique_ptr<ebpf::BPF>(new ebpf::BPF(0, nullptr, true, name, true, nullptr, false));
  master_program_->init(BASECUBE_MASTER_CODE + master_code, cflags_);

  // get references to those maps
  auto ingress_ = master_program_->get_prog_table("ingress_programs");
  ingress_programs_table_ =
      std::unique_ptr<ebpf::BPFProgTable>(new ebpf::BPFProgTable(ingress_));

  auto egress_ = master_program_->get_prog_table("egress_programs");
  egress_programs_table_ =
      std::unique_ptr<ebpf::BPFProgTable>(new ebpf::BPFProgTable(egress_));
}

void BaseCube::init(const std::vector<std::string> &ingress_code,
                    const std::vector<std::string> &egress_code) {
  for (int i = 0; i < ingress_code.size(); i++) {
    add_program(ingress_code[i], i, ProgramType::INGRESS);
  }

  for (int i = 0; i < egress_code.size(); i++) {
    add_program(egress_code[i], i, ProgramType::EGRESS);
  }
}

void BaseCube::uninit() {
  if (ingress_index_)
    patch_panel_.remove(ingress_index_);

  if (egress_index_)
    patch_panel_.remove(egress_index_);

  for (int i = 0; i < ingress_programs_.size(); i++) {
    if (ingress_programs_[i]) {
      del_program(i, ProgramType::INGRESS);
    }
  }

  for (int i = 0; i < egress_programs_.size(); i++) {
    if (egress_programs_[i]) {
      del_program(i, ProgramType::EGRESS);
    }
  }
}

BaseCube::~BaseCube() {
  id_generator_.release(id_);
}

uint32_t BaseCube::get_id() const {
  return id_;
}

uint16_t BaseCube::get_index(ProgramType type) const {
  switch (type) {
  case ProgramType::INGRESS:
    return ingress_index_;
  case ProgramType::EGRESS:
    return egress_index_;
  }
  return 0;
}

CubeType BaseCube::get_type() const {
  return type_;
}

const std::string BaseCube::get_name() const {
  return name_;
}

const std::string BaseCube::get_service_name() const {
  return service_name_;
}

const Guid &BaseCube::uuid() const {
  return uuid_;
}

const bool BaseCube::get_dyn_opt_enabled() const {
  return dyn_opt_enabled_;
}

const bool BaseCube::get_morpheus_started() const {
  return morpheus_started_;
}

void BaseCube::set_start_morpheus(bool start) {
  if (start && !morpheus_started_) {
    std::lock_guard<std::mutex> cube_guard(cube_mutex_);
    for (int i = 0; i < ingress_programs_.size(); i++) {
      if (ingress_programs_[i]) {
        ingress_programs_[i]->enable_morpheus();
      }
    }

    for (int i = 0; i < egress_programs_.size(); i++) {
      if (egress_programs_[i]) {
        egress_programs_[i]->enable_morpheus();
      }
    }

    morpheus_started_ = true;
  }
}

int BaseCube::get_table_fd(const std::string &table_name, int index,
                           ProgramType type) {
  // TODO: Improve bcc api to do it
  switch (type) {
  case ProgramType::INGRESS:
    return ingress_programs_[index]->get_table(table_name).fd();
  case ProgramType::EGRESS:
    return egress_programs_[index]->get_table(table_name).fd();
  }

  throw std::runtime_error("Unknown program type");
}

const ebpf::TableDesc &BaseCube::get_table_desc(const std::string &table_name,
                                         int index, ProgramType type) {
  switch (type) {
    case ProgramType::INGRESS:
      return ingress_programs_[index]->get_table(table_name).getTableDescription();
    case ProgramType::EGRESS:
      return egress_programs_[index]->get_table(table_name).getTableDescription();
  }

  throw std::runtime_error("Unknown program type");
}

void BaseCube::reload(const std::string &code, int index, ProgramType type) {
  std::lock_guard<std::mutex> cube_guard(cube_mutex_);

  switch (type) {
    case ProgramType::INGRESS:
      return do_reload(code, index, type, ingress_programs_, ingress_code_,
                       ingress_programs_table_, ingress_index_);

    case ProgramType::EGRESS:
      return do_reload(code, index, type, egress_programs_, egress_code_,
                       egress_programs_table_, egress_index_);

    default:
      throw std::runtime_error("Bad program type");
  }
}

void BaseCube::reload_all() {
  std::lock_guard<std::mutex> cube_guard(cube_mutex_);
  for (int i = 0; i < ingress_programs_.size(); i++) {
    if (ingress_programs_[i]) {
      do_reload(ingress_code_[i], i, ProgramType::INGRESS, ingress_programs_,
                ingress_code_, ingress_programs_table_, ingress_index_);
    }
  }

  for (int i = 0; i < egress_programs_.size(); i++) {
    if (egress_programs_[i]) {
      do_reload(egress_code_[i], i, ProgramType::EGRESS, egress_programs_,
                egress_code_, egress_programs_table_, egress_index_);
    }
  }
}

void BaseCube::do_reload(
    const std::string &code, int index, ProgramType type,
    std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
        &programs,
    std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
    std::unique_ptr<ebpf::BPFProgTable> &programs_table,
    uint16_t first_program_index) {
  if (index >= programs.size()) {
    throw std::runtime_error("Invalid ebpf program index");
  }

  if (programs.at(index) == nullptr) {
    throw std::runtime_error("ebpf does not exist");
  }

  // create new ebpf program, telling to steal the maps of this program
  std::unique_lock<std::mutex> bcc_guard(bcc_mutex);
  std::unique_ptr<ebpf::BPF> new_bpf_program = std::unique_ptr<ebpf::BPF>(
      new ebpf::BPF(0, nullptr, true, name_, false, programs.at(index).get(), dyn_opt_enabled_,
      std::bind(&BaseCube::handle_dynamic_opt_callback, this, std::placeholders::_1, index, type)));

  bcc_guard.unlock();
  compile(*new_bpf_program, code, index, type);
  int fd = load(*new_bpf_program, type);

  programs_table->update_value(index, fd);

  if (index == 0) {
    patch_panel_.update(first_program_index, fd);
  }

  unload(*programs.at(index), type);
  bcc_guard.lock();
  programs[index] = std::move(new_bpf_program);
  // update last used code
  programs_code[index] = code;
}

bool BaseCube::handle_dynamic_opt_callback(int fd, int index, const ProgramType &type) {
  std::unique_lock<std::mutex> bcc_guard(bcc_mutex);

  switch (type) {
    case ProgramType::INGRESS:
      ingress_programs_table_->update_value(index, fd);
      if (index == 0) {
        if (ingress_index_) {
          // already registered in patch panel, just update
          patch_panel_.update(ingress_index_, fd);
        } else {
          // register in patch panel
          ingress_index_ = patch_panel_.add(fd);
        }
      }
      break;
    case ProgramType::EGRESS:
      egress_programs_table_->update_value(index, fd);
      if (index == 0) {
        if (egress_index_) {
          // already registered in patch panel, just update
          patch_panel_.update(egress_index_, fd);
        } else {
          // register in patch panel
          egress_index_ = patch_panel_.add(fd);
        }
        break;
      }
  }

  // Returning true in this function means that the old program can be safely removed
  return true;
}

void BaseCube::set_cflags(const std::vector<std::string> &cflags) {
  std::lock_guard<std::mutex> lock(bcc_mutex);
  cflags_.clear();

  cflags_.insert(cflags_.end(), default_cflags_.begin(), default_cflags_.end());
  cflags_.insert(cflags_.end(), cflags.begin(), cflags.end());
}

const std::vector<std::string> &BaseCube::get_cflags() {
  return cflags_;
}

int BaseCube::add_program(const std::string &code, int index,
                          ProgramType type) {
  std::lock_guard<std::mutex> cube_guard(cube_mutex_);

  switch (type) {
    case ProgramType::INGRESS:
      return do_add_program(code, index, type, ingress_programs_, ingress_code_,
                            ingress_programs_table_, &ingress_index_);

    case ProgramType::EGRESS:
      return do_add_program(code, index, type,  egress_programs_, egress_code_,
                            egress_programs_table_, &egress_index_);

    default:
      throw std::runtime_error("Bad program type");
  }
}

int BaseCube::do_add_program(
    const std::string &code, int index, ProgramType type,
    std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
        &programs,
    std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
    std::unique_ptr<ebpf::BPFProgTable> &programs_table,
    uint16_t *first_program_index) {

  // check index boundaries
  if (index < -1 || index >= programs.size()) {
    throw std::runtime_error("Invalid ebpf program index");
  }

  if (index != -1) {
    if (programs.at(index) != nullptr) {
      throw std::runtime_error("Program is not empty");
    }

  } else {
    int i;
    for (i = 0; i < programs.size(); i++) {
      if (programs.at(i) == nullptr) {
        index = i;
        break;
      }
    }

    if (i == programs.size()) {
      throw std::runtime_error("Maximum number of programs reached");
    }
  }

  std::unique_lock<std::mutex> bcc_guard(bcc_mutex);
  // load and add this program to the list
  int fd_;
  programs[index] =
      std::make_unique<ebpf::BPF>(0, nullptr, true,
              name_, true, nullptr, dyn_opt_enabled_,
              std::bind(&BaseCube::handle_dynamic_opt_callback, this, std::placeholders::_1, index, type));

  bcc_guard.unlock();
  compile(*programs.at(index), code, index, type);
  int fd = load(*programs.at(index), type);
  bcc_guard.lock();

  programs_table->update_value(index, fd);
  if (index == 0) {
    if (*first_program_index) {
      // already registed in patch panel, just update
      patch_panel_.update(*first_program_index, fd);
    } else {
      // register in patch panel
      *first_program_index = patch_panel_.add(fd);
    }
  }

  programs_code[index] = code;

  return index;
}

void BaseCube::del_program(int index, ProgramType type) {
  std::lock_guard<std::mutex> cube_guard(cube_mutex_);

  switch (type) {
    case ProgramType::INGRESS:
      do_del_program(index, type, ingress_programs_, ingress_code_,
                     ingress_programs_table_);
      break;

    case ProgramType::EGRESS:
      do_del_program(index, type, egress_programs_, egress_code_,
                     egress_programs_table_);
      break;

    default:
      throw std::runtime_error("Bad program type");
  }
}

void BaseCube::do_del_program(
    int index, ProgramType type,
    std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
        &programs,
    std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
    std::unique_ptr<ebpf::BPFProgTable> &programs_table) {
  if (index < 0 || index >= programs.size()) {
    logger->error("Invalid ebpf program index");
    throw std::runtime_error("Invalid ebpf program index");
  }

  if (programs.at(index) == nullptr) {
    throw std::runtime_error("ebpf program does not exist");
  }

  programs_table->remove_value(index);
  unload(*(programs.at(index)), type);
  std::unique_lock<std::mutex> bcc_guard(bcc_mutex);
  programs.at(index).reset();
  programs_code.at(index).clear();
}

std::string BaseCube::get_wrapper_code() {
  return BASECUBE_WRAPPER;
}

void BaseCube::set_log_level(LogLevel level) {
  if (level_ == level)
    return;

  // change log level in dataplane
  log_level_cb_(level);

  level_ = level;
  reload_all();
}

LogLevel BaseCube::get_log_level() const {
  return level_;
}

void BaseCube::set_log_level_cb(const polycube::service::set_log_level_cb &cb) {
  log_level_cb_ = cb;
}

void BaseCube::set_conf(const nlohmann::json &conf) {
  if (conf.count("loglevel")) {
    set_log_level(stringLogLevel(conf.at("loglevel").get<std::string>()));
  }
}

nlohmann::json BaseCube::to_json() const {
  nlohmann::json j;

  j["name"] = name_;
  j["uuid"] = uuid_.str();
  j["service-name"] = service_name_;
  j["type"] = cube_type_to_string(type_);
  j["loglevel"] = logLevelString(level_);
  j["dyn-opt"] = dyn_opt_enabled_;
  j["start-morpheus"] = morpheus_started_;

  return j;
}

IDGenerator BaseCube::id_generator_(PatchPanel::_POLYCUBE_MAX_NODES - 2);

const std::string BaseCube::BASECUBE_MASTER_CODE = R"(
// Tables to save file descriptor of ingress and egress programs
// In TC cubes only egress_programs is used
// In XDP cubes egress_programs holds fds of TC programs, while
// egress_programs_xdp holds XDP programs
BPF_TABLE_SHARED("prog", int, int, ingress_programs,
                 _POLYCUBE_MAX_BPF_PROGRAMS);
BPF_TABLE_SHARED("prog", int, int, egress_programs, _POLYCUBE_MAX_BPF_PROGRAMS);
BPF_TABLE_SHARED("prog", int, int, egress_programs_xdp,
                 _POLYCUBE_MAX_BPF_PROGRAMS);
)";

const std::string BaseCube::BASECUBE_WRAPPER = R"(
#include <bcc/helpers.h>
#include <bcc/proto.h>
#include <uapi/linux/pkt_cls.h>

#include <uapi/linux/bpf.h>

#define CONTROLLER_MODULE_INDEX (_POLYCUBE_MAX_NODES - 1)

// maps definitions, same as in master program but "extern"
BPF_TABLE("extern", int, int, ingress_programs, _POLYCUBE_MAX_BPF_PROGRAMS);
BPF_TABLE("extern", int, int, egress_programs, _POLYCUBE_MAX_BPF_PROGRAMS);
BPF_TABLE("extern", int, int, egress_programs_xdp, _POLYCUBE_MAX_BPF_PROGRAMS);

enum {
  RX_OK,
  RX_REDIRECT,
  RX_DROP,
  RX_RECIRCULATE,
  RX_ERROR,
};

struct pkt_metadata {
  u16 cube_id;        //__attribute__((deprecated)) // use CUBE_ID instead
  u16 in_port;        // The interface on which a packet was received.
  u32 packet_len;     //__attribute__((deprecated)) // Use ctx->len
  u32 traffic_class;  // The traffic class the packet belongs to

  // used to send data to controller
  u16 reason;
  u32 md[3];
} __attribute__((packed));

// cube must implement this function to attach to the networking stack
static __always_inline
int handle_rx(struct CTXTYPE *ctx, struct pkt_metadata *md);

static __always_inline
int pcn_pkt_redirect(struct CTXTYPE *skb,
                     struct pkt_metadata *md, u32 port);

static __always_inline
int pcn_pkt_drop(struct CTXTYPE *skb, struct pkt_metadata *md);

static __always_inline
void pcn_pkt_recirculate(struct CTXTYPE *skb, u32 port);

static __always_inline
int pcn_pkt_controller(struct CTXTYPE *skb, struct pkt_metadata *md,
                       u16 reason);

static __always_inline
int pcn_pkt_controller_with_metadata(struct CTXTYPE *skb,
                                     struct pkt_metadata *md,
                                     u16 reason,
                                     u32 metadata[3]);

static __always_inline
void call_ingress_program(struct CTXTYPE *skb, int index) {
  ingress_programs.call(skb, index);
}

static __always_inline
void call_ingress_program_with_metadata(struct CTXTYPE *skb,
                                        struct pkt_metadata *md, int index);

static __always_inline
void call_egress_program(struct CTXTYPE *skb, int index) {
#ifdef POLYCUBE_XDP
  egress_programs_xdp.call(skb, index);
#else
  egress_programs.call(skb, index);
#endif
}

static __always_inline
void call_egress_program_with_metadata(struct CTXTYPE *skb,
                                       struct pkt_metadata *md, int index);

/* checksum related */

// those functions have different implementations for XDP and TC
static __always_inline
int pcn_l3_csum_replace(struct CTXTYPE *ctx, u32 csum_offset,
                        u32 old_value, u32 new_value, u32 flags);

static __always_inline
int pcn_l4_csum_replace(struct CTXTYPE *ctx, u32 csum_offset,
                        u32 old_value, u32 new_value, u32 flags);

static __always_inline
__wsum pcn_csum_diff(__be32 *from, u32 from_size, __be32 *to,
                     u32 to_size, __wsum seed);

/* vlan related */
static __always_inline
bool pcn_is_vlan_present(struct CTXTYPE *pkt);

static __always_inline
int pcn_get_vlan_id(struct CTXTYPE *pkt);

static __always_inline
int pcn_get_vlan_proto(struct CTXTYPE *pkt);

static __always_inline
int pcn_vlan_pop_tag(struct CTXTYPE *pkt);

static __always_inline
int pcn_vlan_push_tag(struct CTXTYPE *pkt, u16 eth_proto, u32 vlan_id);

/* helper to get time related to Unix Epoch */
static __always_inline
uint64_t pcn_get_time_epoch() {
  return _EPOCH_BASE + bpf_ktime_get_ns();
}

typedef __u8  __attribute__((__may_alias__))  __u8_alias_t;
typedef __u16 __attribute__((__may_alias__)) __u16_alias_t;
typedef __u32 __attribute__((__may_alias__)) __u32_alias_t;
typedef __u64 __attribute__((__may_alias__)) __u64_alias_t;

static __always_inline void __read_once_size_custom(const volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(__u8_alias_t  *) res = *(volatile __u8_alias_t  *) p; break;
	case 2: *(__u16_alias_t *) res = *(volatile __u16_alias_t *) p; break;
	case 4: *(__u32_alias_t *) res = *(volatile __u32_alias_t *) p; break;
	case 8: *(__u64_alias_t *) res = *(volatile __u64_alias_t *) p; break;
	default:
		asm volatile ("" : : : "memory");
		__builtin_memcpy((void *)res, (const void *)p, size);
		asm volatile ("" : : : "memory");
	}
}

static __always_inline void __write_once_size_custom(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile  __u8_alias_t *) p = *(__u8_alias_t  *) res; break;
	case 2: *(volatile __u16_alias_t *) p = *(__u16_alias_t *) res; break;
	case 4: *(volatile __u32_alias_t *) p = *(__u32_alias_t *) res; break;
	case 8: *(volatile __u64_alias_t *) p = *(__u64_alias_t *) res; break;
	default:
		asm volatile ("" : : : "memory");
		__builtin_memcpy((void *)p, (const void *)res, size);
		asm volatile ("" : : : "memory");
	}
}

#define READ_ONCE(x)					\
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__c = { 0 } };			\
	__read_once_size_custom(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})

#define WRITE_ONCE(x, val)				\
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = (val) }; 			\
	__write_once_size_custom(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})

#define NO_TEAR_ADD(x, val) WRITE_ONCE((x), READ_ONCE(x) + (val))
#define NO_TEAR_INC(x) NO_TEAR_ADD((x), 1)
)";

}  // namespace polycubed
}  // namespace polycube
