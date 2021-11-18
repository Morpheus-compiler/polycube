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

#pragma once

#include "bcc_mutex.h"
#include "id_generator.h"
#include "patchpanel.h"
#include "polycube/services/cube_factory.h"
#include "polycube/services/cube_iface.h"
#include "polycube/services/guid.h"
#include "polycube/services/json.hpp"

#include <api/BPF.h>
#include <api/BPFTable.h>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <exception>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using polycube::service::BaseCubeIface;
using polycube::service::PortIface;
using polycube::service::ProgramType;
using polycube::service::CubeType;

using json = nlohmann::json;

namespace polycube::polycubed {

class BaseCube : virtual public BaseCubeIface {
 public:
  explicit BaseCube(const std::string &name, const std::string &service_name,
                    const std::string &master_code, PatchPanel &patch_panel,
                    LogLevel level, CubeType type, bool dyn_opt_enabled, const std::vector<std::string> &cflags);
  virtual ~BaseCube();

  // It is not possible to copy nor assign nor move an cube.
  BaseCube(const BaseCube &) = delete;
  BaseCube &operator=(const BaseCube &) = delete;
  BaseCube(BaseCube &&a) = delete;

  virtual void reload(const std::string &code, int index, ProgramType type);
  virtual int add_program(const std::string &code, int index, ProgramType type);
  virtual void del_program(int index, ProgramType type);

  static std::string get_wrapper_code();

  CubeType get_type() const override;
  uint32_t get_id() const override;
  uint16_t get_index(ProgramType type) const override;
  const std::string get_name() const override;
  const std::string get_service_name() const override;
  const bool get_dyn_opt_enabled() const override;
  const Guid &uuid() const override;

  int get_table_fd(const std::string &table_name, int index, ProgramType type);
  const ebpf::TableDesc &get_table_desc(const std::string &table_name, int index,
                             ProgramType type);

  void set_log_level(LogLevel level) override;
  LogLevel get_log_level() const override;

  void set_conf(const nlohmann::json &conf) override;
  virtual nlohmann::json to_json() const override;

  void set_cflags(const std::vector<std::string> &cflags);
  const std::vector<std::string> &get_cflags();

  void set_log_level_cb(const polycube::service::set_log_level_cb &cb);

  bool handle_dynamic_opt_callback(int fd, int index, const ProgramType &type);

  const bool get_morpheus_started() const override;
  void set_start_morpheus(bool start) override;

 protected:
  static const int _POLYCUBE_MAX_BPF_PROGRAMS = 64;
  static const int _POLYCUBE_MAX_PORTS = 128;
  static_assert(_POLYCUBE_MAX_PORTS <= 0xffff,
          "_POLYCUBE_MAX_PORTS shouldn't be great than 0xffff, "
          "id 0xffff was used by iptables wild card index");
  static std::vector<std::string> default_cflags_;
  std::vector<std::string> cflags_;

  virtual int load(ebpf::BPF &bpf, ProgramType type) = 0;
  virtual void unload(ebpf::BPF &bpf, ProgramType type) = 0;
  virtual void compile(ebpf::BPF &bpf, const std::string &code, int index,
                       ProgramType type) = 0;

  void init(const std::vector<std::string> &ingress_code,
            const std::vector<std::string> &egress_code);
  void uninit();

  virtual void reload_all();

  void do_reload(
    const std::string &code, int index, ProgramType type,
    std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
        &programs,
    std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
    std::unique_ptr<ebpf::BPFProgTable> &programs_table,
    uint16_t first_program_index);
  int do_add_program(
      const std::string &code, int index, ProgramType type,
      std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
          &programs,
      std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
      std::unique_ptr<ebpf::BPFProgTable> &programs_table,
      uint16_t *first_program_index);
  void do_del_program(
    int index, ProgramType type,
    std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
        &programs,
    std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> &programs_code,
    std::unique_ptr<ebpf::BPFProgTable> &programs_table);

  bool do_start_morpheus(bool start);

  PatchPanel &patch_panel_;

  CubeType type_;
  std::string name_;
  std::string service_name_;
  Guid uuid_;
  uint32_t id_;

  uint16_t ingress_index_;
  uint16_t egress_index_;

  std::unique_ptr<ebpf::BPF> master_program_;

  std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
      ingress_programs_;
  std::array<std::unique_ptr<ebpf::BPF>, _POLYCUBE_MAX_BPF_PROGRAMS>
      egress_programs_;

  std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> ingress_code_;
  std::array<std::string, _POLYCUBE_MAX_BPF_PROGRAMS> egress_code_;

  std::unique_ptr<ebpf::BPFProgTable> ingress_programs_table_;
  std::unique_ptr<ebpf::BPFProgTable> egress_programs_table_;

  std::shared_ptr<spdlog::logger> logger;
  LogLevel level_;

  bool dyn_opt_enabled_;
  bool morpheus_started_;

  std::mutex cube_mutex_;  // blocks operations over the whole cube
 private:
  // ebpf wrappers
  static const std::string BASECUBE_MASTER_CODE;
  static const std::string BASECUBE_WRAPPER;

  static IDGenerator id_generator_;

  polycube::service::set_log_level_cb log_level_cb_;
};

}  // namespace polycube
