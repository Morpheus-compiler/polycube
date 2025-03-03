/**
* simpleforwarder API generated from simpleforwarder.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "SimpleforwarderApiImpl.h"

namespace polycube {
namespace service {
namespace api {

using namespace polycube::service::model;

namespace SimpleforwarderApiImpl {
namespace {
std::unordered_map<std::string, std::shared_ptr<Simpleforwarder>> cubes;
std::mutex cubes_mutex;

std::shared_ptr<Simpleforwarder> get_cube(const std::string &name) {
  std::lock_guard<std::mutex> guard(cubes_mutex);
  auto iter = cubes.find(name);
  if (iter == cubes.end()) {
    throw std::runtime_error("Cube " + name + " does not exist");
  }

  return iter->second;
}

}

void create_simpleforwarder_by_id(const std::string &name, const SimpleforwarderJsonObject &jsonObject) {
  {
    // check if name is valid before creating it
    std::lock_guard<std::mutex> guard(cubes_mutex);
    if (cubes.count(name) != 0) {
      throw std::runtime_error("There is already a cube with name " + name);
    }
  }
  auto ptr = std::make_shared<Simpleforwarder>(name, jsonObject);
  std::unordered_map<std::string, std::shared_ptr<Simpleforwarder>>::iterator iter;
  bool inserted;

  std::lock_guard<std::mutex> guard(cubes_mutex);
  std::tie(iter, inserted) = cubes.emplace(name, std::move(ptr));

  if (!inserted) {
    throw std::runtime_error("There is already a cube with name " + name);
  }
}

void replace_simpleforwarder_by_id(const std::string &name, const SimpleforwarderJsonObject &bridge){
  throw std::runtime_error("Method not supported!");
}

void delete_simpleforwarder_by_id(const std::string &name) {
  std::lock_guard<std::mutex> guard(cubes_mutex);
  if (cubes.count(name) == 0) {
    throw std::runtime_error("Cube " + name + " does not exist");
  }
  cubes.erase(name);
}

std::vector<SimpleforwarderJsonObject> read_simpleforwarder_list_by_id() {
  std::vector<SimpleforwarderJsonObject> jsonObject_vect;
  for(auto &i : cubes) {
    auto m = get_cube(i.first);
    jsonObject_vect.push_back(m->toJsonObject());
  }
  return jsonObject_vect;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_simpleforwarder_list_by_id_get_list() {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  for (auto &x : cubes) {
    nlohmann::fifo_map<std::string, std::string> m;
    m["name"] = x.first;
    r.push_back(std::move(m));
  }
  return r;
}

/**
* @brief   Create actions by ID
*
* Create operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
create_simpleforwarder_actions_by_id(const std::string &name, const std::string &inport, const ActionsJsonObject &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->addActions(inport, value);
}

/**
* @brief   Create actions by ID
*
* Create operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
create_simpleforwarder_actions_list_by_id(const std::string &name, const std::vector<ActionsJsonObject> &value) {
  auto simpleforwarder = get_cube(name);
  simpleforwarder->addActionsList(value);
}

/**
* @brief   Create ports by ID
*
* Create operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
create_simpleforwarder_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->addPorts(portsName, value);
}

/**
* @brief   Create ports by ID
*
* Create operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
create_simpleforwarder_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  auto simpleforwarder = get_cube(name);
  simpleforwarder->addPortsList(value);
}

/**
* @brief   Delete actions by ID
*
* Delete operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
*
* Responses:
*
*/
void
delete_simpleforwarder_actions_by_id(const std::string &name, const std::string &inport) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->delActions(inport);
}

/**
* @brief   Delete actions by ID
*
* Delete operation of resource: actions*
*
* @param[in] name ID of name
*
* Responses:
*
*/
void
delete_simpleforwarder_actions_list_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  simpleforwarder->delActionsList();
}

/**
* @brief   Delete ports by ID
*
* Delete operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
*
* Responses:
*
*/
void
delete_simpleforwarder_ports_by_id(const std::string &name, const std::string &portsName) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->delPorts(portsName);
}

/**
* @brief   Delete ports by ID
*
* Delete operation of resource: ports*
*
* @param[in] name ID of name
*
* Responses:
*
*/
void
delete_simpleforwarder_ports_list_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  simpleforwarder->delPortsList();
}

/**
* @brief   Read action by ID
*
* Read operation of resource: action*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
*
* Responses:
* ActionsActionEnum
*/
ActionsActionEnum
read_simpleforwarder_actions_action_by_id(const std::string &name, const std::string &inport) {
  auto simpleforwarder = get_cube(name);
  auto actions = simpleforwarder->getActions(inport);
  return actions->getAction();

}

/**
* @brief   Read actions by ID
*
* Read operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
*
* Responses:
* ActionsJsonObject
*/
ActionsJsonObject
read_simpleforwarder_actions_by_id(const std::string &name, const std::string &inport) {
  auto simpleforwarder = get_cube(name);
  return simpleforwarder->getActions(inport)->toJsonObject();

}

/**
* @brief   Read actions by ID
*
* Read operation of resource: actions*
*
* @param[in] name ID of name
*
* Responses:
* std::vector<ActionsJsonObject>
*/
std::vector<ActionsJsonObject>
read_simpleforwarder_actions_list_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  auto &&actions = simpleforwarder->getActionsList();
  std::vector<ActionsJsonObject> m;
  for(auto &i : actions)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read outport by ID
*
* Read operation of resource: outport*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
*
* Responses:
* std::string
*/
std::string
read_simpleforwarder_actions_outport_by_id(const std::string &name, const std::string &inport) {
  auto simpleforwarder = get_cube(name);
  auto actions = simpleforwarder->getActions(inport);
  return actions->getOutport();

}

/**
* @brief   Read simpleforwarder by ID
*
* Read operation of resource: simpleforwarder*
*
* @param[in] name ID of name
*
* Responses:
* SimpleforwarderJsonObject
*/
SimpleforwarderJsonObject
read_simpleforwarder_by_id(const std::string &name) {
  return get_cube(name)->toJsonObject();

}

/**
* @brief   Read dyn-opt by ID
*
* Read operation of resource: dyn-opt*
*
* @param[in] name ID of name
*
* Responses:
* bool
*/
bool
read_simpleforwarder_dyn_opt_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  return simpleforwarder->getDynOpt();

}

/**
* @brief   Read ports by ID
*
* Read operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
*
* Responses:
* PortsJsonObject
*/
PortsJsonObject
read_simpleforwarder_ports_by_id(const std::string &name, const std::string &portsName) {
  auto simpleforwarder = get_cube(name);
  return simpleforwarder->getPorts(portsName)->toJsonObject();

}

/**
* @brief   Read ip by ID
*
* Read operation of resource: ip*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
*
* Responses:
* std::string
*/
std::string
read_simpleforwarder_ports_ip_by_id(const std::string &name, const std::string &portsName) {
  auto simpleforwarder = get_cube(name);
  auto ports = simpleforwarder->getPorts(portsName);
  return ports->getIp();

}

/**
* @brief   Read ports by ID
*
* Read operation of resource: ports*
*
* @param[in] name ID of name
*
* Responses:
* std::vector<PortsJsonObject>
*/
std::vector<PortsJsonObject>
read_simpleforwarder_ports_list_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  auto &&ports = simpleforwarder->getPortsList();
  std::vector<PortsJsonObject> m;
  for(auto &i : ports)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read simpleRedirect by ID
*
* Read operation of resource: simpleRedirect*
*
* @param[in] name ID of name
*
* Responses:
* bool
*/
bool
read_simpleforwarder_simple_redirect_by_id(const std::string &name) {
  auto simpleforwarder = get_cube(name);
  return simpleforwarder->getSimpleRedirect();

}

/**
* @brief   Replace actions by ID
*
* Replace operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
replace_simpleforwarder_actions_by_id(const std::string &name, const std::string &inport, const ActionsJsonObject &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->replaceActions(inport, value);
}

/**
* @brief   Replace actions by ID
*
* Replace operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
replace_simpleforwarder_actions_list_by_id(const std::string &name, const std::vector<ActionsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Replace ports by ID
*
* Replace operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
replace_simpleforwarder_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->replacePorts(portsName, value);
}

/**
* @brief   Replace ports by ID
*
* Replace operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
replace_simpleforwarder_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update action by ID
*
* Update operation of resource: action*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
* @param[in] value Action associated to the current table entry (i.e., DROP, SLOWPATH, or FORWARD; default: DROP)
*
* Responses:
*
*/
void
update_simpleforwarder_actions_action_by_id(const std::string &name, const std::string &inport, const ActionsActionEnum &value) {
  auto simpleforwarder = get_cube(name);
  auto actions = simpleforwarder->getActions(inport);

  return actions->setAction(value);
}

/**
* @brief   Update actions by ID
*
* Update operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
update_simpleforwarder_actions_by_id(const std::string &name, const std::string &inport, const ActionsJsonObject &value) {
  auto simpleforwarder = get_cube(name);
  auto actions = simpleforwarder->getActions(inport);

  return actions->update(value);
}

/**
* @brief   Update actions by ID
*
* Update operation of resource: actions*
*
* @param[in] name ID of name
* @param[in] value actionsbody object
*
* Responses:
*
*/
void
update_simpleforwarder_actions_list_by_id(const std::string &name, const std::vector<ActionsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update outport by ID
*
* Update operation of resource: outport*
*
* @param[in] name ID of name
* @param[in] inport ID of inport
* @param[in] value Output port (used only when action is FORWARD)
*
* Responses:
*
*/
void
update_simpleforwarder_actions_outport_by_id(const std::string &name, const std::string &inport, const std::string &value) {
  auto simpleforwarder = get_cube(name);
  auto actions = simpleforwarder->getActions(inport);

  return actions->setOutport(value);
}

/**
* @brief   Update simpleforwarder by ID
*
* Update operation of resource: simpleforwarder*
*
* @param[in] name ID of name
* @param[in] value simpleforwarderbody object
*
* Responses:
*
*/
void
update_simpleforwarder_by_id(const std::string &name, const SimpleforwarderJsonObject &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->update(value);
}

/**
* @brief   Update simpleforwarder by ID
*
* Update operation of resource: simpleforwarder*
*
* @param[in] value simpleforwarderbody object
*
* Responses:
*
*/
void
update_simpleforwarder_list_by_id(const std::vector<SimpleforwarderJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update ports by ID
*
* Update operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
update_simpleforwarder_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto simpleforwarder = get_cube(name);
  auto ports = simpleforwarder->getPorts(portsName);

  return ports->update(value);
}

/**
* @brief   Update ip by ID
*
* Update operation of resource: ip*
*
* @param[in] name ID of name
* @param[in] portsName ID of ports_name
* @param[in] value IP address and prefix of the port
*
* Responses:
*
*/
void
update_simpleforwarder_ports_ip_by_id(const std::string &name, const std::string &portsName, const std::string &value) {
  auto simpleforwarder = get_cube(name);
  auto ports = simpleforwarder->getPorts(portsName);

  return ports->setIp(value);
}

/**
* @brief   Update ports by ID
*
* Update operation of resource: ports*
*
* @param[in] name ID of name
* @param[in] value portsbody object
*
* Responses:
*
*/
void
update_simpleforwarder_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update simpleRedirect by ID
*
* Update operation of resource: simpleRedirect*
*
* @param[in] name ID of name
* @param[in] value Flag to indicate if a simple redirect in the other port should be performed
*
* Responses:
*
*/
void
update_simpleforwarder_simple_redirect_by_id(const std::string &name, const bool &value) {
  auto simpleforwarder = get_cube(name);

  return simpleforwarder->setSimpleRedirect(value);
}



/*
 * help related
 */

std::vector<nlohmann::fifo_map<std::string, std::string>> read_simpleforwarder_actions_list_by_id_get_list(const std::string &name) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&simpleforwarder = get_cube(name);

  auto &&actions = simpleforwarder->getActionsList();
  for(auto &i : actions) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["inport"] = i->getInport();

    r.push_back(keys);
  }
  return r;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_simpleforwarder_ports_list_by_id_get_list(const std::string &name) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&simpleforwarder = get_cube(name);

  auto &&ports = simpleforwarder->getPortsList();
  for(auto &i : ports) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["name"] = i->getName();

    r.push_back(keys);
  }
  return r;
}


}

}
}
}

