/**
* bmc API generated from bmc.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


#pragma once


#include "../base/BmcBase.h"

#include "BmcStats.h"
#include "Memcached.h"
#include "Ports.h"

#include <linux/bpf.h>

#define BMC_MAX_KEY_LENGTH 250
#define BMC_MAX_VAL_LENGTH 1000
#define BMC_MAX_ADDITIONAL_PAYLOAD_BYTES 53
#define BMC_MAX_CACHE_DATA_SIZE BMC_MAX_KEY_LENGTH+BMC_MAX_VAL_LENGTH+BMC_MAX_ADDITIONAL_PAYLOAD_BYTES
#define BMC_MAX_KEY_IN_MULTIGET 30
#define BMC_CACHE_ENTRY_COUNT 3250000
#define BMC_MAX_PACKET_LENGTH 1500
#define BMC_MAX_KEY_IN_PACKET BMC_MAX_KEY_IN_MULTIGET


using namespace polycube::service::model;

class Bmc : public BmcBase {
 public:
  Bmc(const std::string name, const BmcJsonObject &conf);
  virtual ~Bmc();

  void packet_in(Ports &port,
                 polycube::service::PacketInMetadata &md,
                 const std::vector<uint8_t> &packet) override;

  /// <summary>
  /// Defines if the service should be automatically optimized by Morpheus
  /// </summary>
  bool getDynOpt() override;

  /// <summary>
  /// Entry of the ports table
  /// </summary>
  std::shared_ptr<Ports> getPorts(const std::string &name) override;
  std::vector<std::shared_ptr<Ports>> getPortsList() override;
  void addPorts(const std::string &name, const PortsJsonObject &conf) override;
  void addPortsList(const std::vector<PortsJsonObject> &conf) override;
  void replacePorts(const std::string &name, const PortsJsonObject &conf) override;
  void delPorts(const std::string &name) override;
  void delPortsList() override;

  /// <summary>
  ///
  /// </summary>
  std::shared_ptr<BmcStats> getBmcStats() override;
  void addBmcStats(const BmcStatsJsonObject &value) override;
  void replaceBmcStats(const BmcStatsJsonObject &conf) override;
  void delBmcStats() override;

  /// <summary>
  /// Entry in the BMC Cache
  /// </summary>
  std::shared_ptr<Memcached> getMemcached(const uint32_t &hashKey) override;
  std::vector<std::shared_ptr<Memcached>> getMemcachedList() override;
  void addMemcached(const uint32_t &hashKey, const MemcachedJsonObject &conf) override;
  void addMemcachedList(const std::vector<MemcachedJsonObject> &conf) override;
  void replaceMemcached(const uint32_t &hashKey, const MemcachedJsonObject &conf) override;
  void delMemcached(const uint32_t &hashKey) override;
  void delMemcachedList() override;

private:
  static std::vector<std::string> COMPILE_FLAGS;
  enum XDP_PROGRAMS {
    BMC_PROG_XDP_RX_FILTER = 0,
    BMC_PROG_XDP_HASH_KEYS,
    BMC_PROG_XDP_PREPARE_PACKET,
    BMC_PROG_XDP_WRITE_REPLY,
    BMC_PROG_XDP_INVALIDATE_CACHE
  };

  enum TC_PROGRAMS{
	  BMC_PROG_TC_TX_FILTER = 0,
	  BMC_PROG_TC_UPDATE_CACHE
  };

  struct bmc_cache_entry {
    struct bpf_spin_lock lock;
    unsigned int len;
    char valid;
    int hash;
    char data[BMC_MAX_CACHE_DATA_SIZE];
  };
};
