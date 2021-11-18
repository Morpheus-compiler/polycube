/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* KatranJsonObject.h
*
*
*/

#pragma once


#include "JsonObjectBase.h"

#include "PortsJsonObject.h"
#include "GeneralStatsJsonObject.h"
#include "VipJsonObject.h"
#include <vector>
#include "polycube/services/cube.h"

namespace polycube {
namespace service {
namespace model {


/// <summary>
///
/// </summary>
class  KatranJsonObject : public JsonObjectBase {
public:
  KatranJsonObject();
  KatranJsonObject(const nlohmann::json &json);
  ~KatranJsonObject() final = default;
  nlohmann::json toJson() const final;


  /// <summary>
  /// Name of the katran service
  /// </summary>
  std::string getName() const;
  void setName(std::string value);
  bool nameIsSet() const;

  /// <summary>
  /// Defines if the service should be automatically optimized by Morpheus
  /// </summary>
  bool getDynOpt() const;
  void setDynOpt(bool value);
  bool dynOptIsSet() const;
  void unsetDynOpt();

  /// <summary>
  /// Entry of the ports table
  /// </summary>
  const std::vector<PortsJsonObject>& getPorts() const;
  void addPorts(PortsJsonObject value);
  bool portsIsSet() const;
  void unsetPorts();

  /// <summary>
  ///
  /// </summary>
  GeneralStatsJsonObject getGeneralStats() const;
  void setGeneralStats(GeneralStatsJsonObject value);
  bool generalStatsIsSet() const;
  void unsetGeneralStats();

  /// <summary>
  /// mac address of default router. katran &#39;offloads&#39; forwarding to the top of the rack switch (by simply sending everything to it by default).
  /// </summary>
  std::string getDefaultMac() const;
  void setDefaultMac(std::string value);
  bool defaultMacIsSet() const;

  /// <summary>
  /// Flag to indicate if healthchecking forwarding plane should be enabled or not.
  /// </summary>
  bool getEnableHc() const;
  void setEnableHc(bool value);
  bool enableHcIsSet() const;
  void unsetEnableHc();

  /// <summary>
  /// Maximum number of VIPs supported by katran. It must be in sync with configuration of forwarding plane
  /// </summary>
  uint32_t getMaxVips() const;
  void setMaxVips(uint32_t value);
  bool maxVipsIsSet() const;
  void unsetMaxVips();

  /// <summary>
  /// Maximum number of Real servers. It must be in sync w/ configuration of forwarding plane.
  /// </summary>
  uint32_t getMaxReals() const;
  void setMaxReals(uint32_t value);
  bool maxRealsIsSet() const;
  void unsetMaxReals();

  /// <summary>
  /// Maximum number of Real servers. It must be in sync w/ configuration of forwarding plane.
  /// </summary>
  uint32_t getChRingSize() const;
  void setChRingSize(uint32_t value);
  bool chRingSizeIsSet() const;
  void unsetChRingSize();

  /// <summary>
  /// ID of cpu cores which are responsible for the packet forwarding
  /// </summary>
  std::string getForwardingCores() const;
  void setForwardingCores(std::string value);
  bool forwardingCoresIsSet() const;
  void unsetForwardingCores();

  /// <summary>
  /// ID of the numa node associated to the forwarding cores
  /// </summary>
  std::string getNumaNodes() const;
  void setNumaNodes(std::string value);
  bool numaNodesIsSet() const;
  void unsetNumaNodes();

  /// <summary>
  /// The size of connection table (number of flows which we are going to track)
  /// </summary>
  uint64_t getLruSize() const;
  void setLruSize(uint64_t value);
  bool lruSizeIsSet() const;
  void unsetLruSize();

  /// <summary>
  /// Services (i.e., virtual ip:protocol:port) exported to the client
  /// </summary>
  const std::vector<VipJsonObject>& getVip() const;
  void addVip(VipJsonObject value);
  bool vipIsSet() const;
  void unsetVip();

private:
  std::string m_name;
  bool m_nameIsSet;
  bool m_dynOpt;
  bool m_dynOptIsSet;
  std::vector<PortsJsonObject> m_ports;
  bool m_portsIsSet;
  GeneralStatsJsonObject m_generalStats;
  bool m_generalStatsIsSet;
  std::string m_defaultMac;
  bool m_defaultMacIsSet;
  bool m_enableHc;
  bool m_enableHcIsSet;
  uint32_t m_maxVips;
  bool m_maxVipsIsSet;
  uint32_t m_maxReals;
  bool m_maxRealsIsSet;
  uint32_t m_chRingSize;
  bool m_chRingSizeIsSet;
  std::string m_forwardingCores;
  bool m_forwardingCoresIsSet;
  std::string m_numaNodes;
  bool m_numaNodesIsSet;
  uint64_t m_lruSize;
  bool m_lruSizeIsSet;
  std::vector<VipJsonObject> m_vip;
  bool m_vipIsSet;
};

}
}
}

