/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* KatranApiImpl.h
*
*
*/

#pragma once


#include <memory>
#include <map>
#include <mutex>
#include "../Katran.h"

#include "GeneralStatsJsonObject.h"
#include "GeneralStatsGetIcmpTooBigStatsOutputJsonObject.h"
#include "GeneralStatsGetLruFallbackStatsOutputJsonObject.h"
#include "GeneralStatsGetLruMissStatsOutputJsonObject.h"
#include "GeneralStatsGetLruStatsOutputJsonObject.h"
#include "GeneralStatsGetVipStatsInputJsonObject.h"
#include "GeneralStatsGetVipStatsOutputJsonObject.h"
#include "KatranJsonObject.h"
#include "PortsJsonObject.h"
#include "VipJsonObject.h"
#include "VipRealJsonObject.h"
#include <vector>

namespace polycube {
namespace service {
namespace api {

using namespace polycube::service::model;

namespace KatranApiImpl {
  void create_katran_by_id(const std::string &name, const KatranJsonObject &value);
  GeneralStatsGetIcmpTooBigStatsOutputJsonObject create_katran_general_stats_get_icmp_too_big_stats_by_id(const std::string &name);
  GeneralStatsGetLruFallbackStatsOutputJsonObject create_katran_general_stats_get_lru_fallback_stats_by_id(const std::string &name);
  GeneralStatsGetLruMissStatsOutputJsonObject create_katran_general_stats_get_lru_miss_stats_by_id(const std::string &name);
  GeneralStatsGetLruStatsOutputJsonObject create_katran_general_stats_get_lru_stats_by_id(const std::string &name);
  GeneralStatsGetVipStatsOutputJsonObject create_katran_general_stats_get_vip_stats_by_id(const std::string &name, const GeneralStatsGetVipStatsInputJsonObject &value);
  void create_katran_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value);
  void create_katran_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value);
  void create_katran_vip_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const VipJsonObject &value);
  void create_katran_vip_list_by_id(const std::string &name, const std::vector<VipJsonObject> &value);
  void create_katran_vip_real_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip, const VipRealJsonObject &value);
  void create_katran_vip_real_list_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::vector<VipRealJsonObject> &value);
  void delete_katran_by_id(const std::string &name);
  void delete_katran_ports_by_id(const std::string &name, const std::string &portsName);
  void delete_katran_ports_list_by_id(const std::string &name);
  void delete_katran_vip_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);
  void delete_katran_vip_list_by_id(const std::string &name);
  void delete_katran_vip_real_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip);
  void delete_katran_vip_real_list_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);
  KatranJsonObject read_katran_by_id(const std::string &name);
  uint32_t read_katran_ch_ring_size_by_id(const std::string &name);
  std::string read_katran_default_mac_by_id(const std::string &name);
  bool read_katran_dyn_opt_by_id(const std::string &name);
  bool read_katran_enable_hc_by_id(const std::string &name);
  std::string read_katran_forwarding_cores_by_id(const std::string &name);
  GeneralStatsJsonObject read_katran_general_stats_by_id(const std::string &name);
  uint64_t read_katran_general_stats_v1_by_id(const std::string &name);
  uint64_t read_katran_general_stats_v2_by_id(const std::string &name);
  std::vector<KatranJsonObject> read_katran_list_by_id();
  uint64_t read_katran_lru_size_by_id(const std::string &name);
  uint32_t read_katran_max_reals_by_id(const std::string &name);
  uint32_t read_katran_max_vips_by_id(const std::string &name);
  std::string read_katran_numa_nodes_by_id(const std::string &name);
  PortsJsonObject read_katran_ports_by_id(const std::string &name, const std::string &portsName);
  std::vector<PortsJsonObject> read_katran_ports_list_by_id(const std::string &name);
  VipJsonObject read_katran_vip_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);
  uint32_t read_katran_vip_flags_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);
  std::vector<VipJsonObject> read_katran_vip_list_by_id(const std::string &name);
  VipRealJsonObject read_katran_vip_real_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip);
  uint32_t read_katran_vip_real_flags_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip);
  std::vector<VipRealJsonObject> read_katran_vip_real_list_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);
  uint16_t read_katran_vip_real_weight_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip);
  void replace_katran_by_id(const std::string &name, const KatranJsonObject &value);
  void replace_katran_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value);
  void replace_katran_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value);
  void replace_katran_vip_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const VipJsonObject &value);
  void replace_katran_vip_list_by_id(const std::string &name, const std::vector<VipJsonObject> &value);
  void replace_katran_vip_real_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip, const VipRealJsonObject &value);
  void replace_katran_vip_real_list_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::vector<VipRealJsonObject> &value);
  void update_katran_by_id(const std::string &name, const KatranJsonObject &value);
  void update_katran_ch_ring_size_by_id(const std::string &name, const uint32_t &value);
  void update_katran_default_mac_by_id(const std::string &name, const std::string &value);
  void update_katran_enable_hc_by_id(const std::string &name, const bool &value);
  void update_katran_forwarding_cores_by_id(const std::string &name, const std::string &value);
  void update_katran_list_by_id(const std::vector<KatranJsonObject> &value);
  void update_katran_lru_size_by_id(const std::string &name, const uint64_t &value);
  void update_katran_max_reals_by_id(const std::string &name, const uint32_t &value);
  void update_katran_max_vips_by_id(const std::string &name, const uint32_t &value);
  void update_katran_numa_nodes_by_id(const std::string &name, const std::string &value);
  void update_katran_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value);
  void update_katran_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value);
  void update_katran_vip_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const VipJsonObject &value);
  void update_katran_vip_flags_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const uint32_t &value);
  void update_katran_vip_list_by_id(const std::string &name, const std::vector<VipJsonObject> &value);
  void update_katran_vip_real_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip, const VipRealJsonObject &value);
  void update_katran_vip_real_flags_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip, const uint32_t &value);
  void update_katran_vip_real_list_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::vector<VipRealJsonObject> &value);
  void update_katran_vip_real_weight_by_id(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto, const std::string &ip, const uint16_t &value);

  /* help related */
  std::vector<nlohmann::fifo_map<std::string, std::string>> read_katran_list_by_id_get_list();
  std::vector<nlohmann::fifo_map<std::string, std::string>> read_katran_ports_list_by_id_get_list(const std::string &name);
  std::vector<nlohmann::fifo_map<std::string, std::string>> read_katran_vip_list_by_id_get_list(const std::string &name);
  std::vector<nlohmann::fifo_map<std::string, std::string>> read_katran_vip_real_list_by_id_get_list(const std::string &name, const std::string &address, const uint16_t &port, const VipProtoEnum &proto);

}
}
}
}

