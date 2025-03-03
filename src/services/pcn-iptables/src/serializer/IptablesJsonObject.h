/**
* iptables API generated from iptables.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* IptablesJsonObject.h
*
*
*/

#pragma once


#include "JsonObjectBase.h"

#include "ChainJsonObject.h"
#include "SessionTableJsonObject.h"
#include "PortsJsonObject.h"
#include <vector>
#include "polycube/services/cube.h"

namespace polycube {
namespace service {
namespace model {

enum class IptablesConntrackEnum {
  ON, OFF
};
enum class IptablesHorusEnum {
  ON, OFF
};

/// <summary>
///
/// </summary>
class  IptablesJsonObject : public JsonObjectBase {
public:
  IptablesJsonObject();
  IptablesJsonObject(const nlohmann::json &json);
  ~IptablesJsonObject() final = default;
  nlohmann::json toJson() const final;


  /// <summary>
  /// Name of the iptables service
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
  /// Interactive mode applies new rules immediately; if &#39;false&#39;, the command &#39;apply-rules&#39; has to be used to apply all the rules at once. Default is TRUE.
  /// </summary>
  bool getInteractive() const;
  void setInteractive(bool value);
  bool interactiveIsSet() const;
  void unsetInteractive();

  /// <summary>
  /// Enables the Connection Tracking module. Mandatory if connection tracking rules are needed. Default is ON.
  /// </summary>
  IptablesConntrackEnum getConntrack() const;
  void setConntrack(IptablesConntrackEnum value);
  bool conntrackIsSet() const;
  void unsetConntrack();
  static std::string IptablesConntrackEnum_to_string(const IptablesConntrackEnum &value);
  static IptablesConntrackEnum string_to_IptablesConntrackEnum(const std::string &str);

  /// <summary>
  /// Enables the HORUS optimization. Default is OFF.
  /// </summary>
  IptablesHorusEnum getHorus() const;
  void setHorus(IptablesHorusEnum value);
  bool horusIsSet() const;
  void unsetHorus();
  static std::string IptablesHorusEnum_to_string(const IptablesHorusEnum &value);
  static IptablesHorusEnum string_to_IptablesHorusEnum(const std::string &str);

  /// <summary>
  ///
  /// </summary>
  const std::vector<SessionTableJsonObject>& getSessionTable() const;
  void addSessionTable(SessionTableJsonObject value);
  bool sessionTableIsSet() const;
  void unsetSessionTable();

  /// <summary>
  ///
  /// </summary>
  const std::vector<ChainJsonObject>& getChain() const;
  void addChain(ChainJsonObject value);
  bool chainIsSet() const;
  void unsetChain();

private:
  std::string m_name;
  bool m_nameIsSet;
  bool m_dynOpt;
  bool m_dynOptIsSet;
  std::vector<PortsJsonObject> m_ports;
  bool m_portsIsSet;
  bool m_interactive;
  bool m_interactiveIsSet;
  IptablesConntrackEnum m_conntrack;
  bool m_conntrackIsSet;
  IptablesHorusEnum m_horus;
  bool m_horusIsSet;
  std::vector<SessionTableJsonObject> m_sessionTable;
  bool m_sessionTableIsSet;
  std::vector<ChainJsonObject> m_chain;
  bool m_chainIsSet;
};

}
}
}

