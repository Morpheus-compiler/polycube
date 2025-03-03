/**
* simpleforwarder API generated from simpleforwarder.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* SimpleforwarderJsonObject.h
*
*
*/

#pragma once


#include "JsonObjectBase.h"

#include "ActionsJsonObject.h"
#include "PortsJsonObject.h"
#include <vector>
#include "polycube/services/cube.h"

namespace polycube {
namespace service {
namespace model {


/// <summary>
///
/// </summary>
class  SimpleforwarderJsonObject : public JsonObjectBase {
public:
  SimpleforwarderJsonObject();
  SimpleforwarderJsonObject(const nlohmann::json &json);
  ~SimpleforwarderJsonObject() final = default;
  nlohmann::json toJson() const final;


  /// <summary>
  /// Name of the simpleforwarder service
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
  /// Flag to indicate if a simple redirect in the other port should be performed
  /// </summary>
  bool getSimpleRedirect() const;
  void setSimpleRedirect(bool value);
  bool simpleRedirectIsSet() const;
  void unsetSimpleRedirect();

  /// <summary>
  /// Entry of the Actions table
  /// </summary>
  const std::vector<ActionsJsonObject>& getActions() const;
  void addActions(ActionsJsonObject value);
  bool actionsIsSet() const;
  void unsetActions();

private:
  std::string m_name;
  bool m_nameIsSet;
  bool m_dynOpt;
  bool m_dynOptIsSet;
  std::vector<PortsJsonObject> m_ports;
  bool m_portsIsSet;
  bool m_simpleRedirect;
  bool m_simpleRedirectIsSet;
  std::vector<ActionsJsonObject> m_actions;
  bool m_actionsIsSet;
};

}
}
}

