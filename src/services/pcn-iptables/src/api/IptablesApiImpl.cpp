/**
* iptables API generated from iptables.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "IptablesApiImpl.h"

namespace polycube {
namespace service {
namespace api {

using namespace polycube::service::model;

namespace IptablesApiImpl {
namespace {
std::unordered_map<std::string, std::shared_ptr<Iptables>> cubes;
std::mutex cubes_mutex;

std::shared_ptr<Iptables> get_cube(const std::string &name) {
  std::lock_guard<std::mutex> guard(cubes_mutex);
  auto iter = cubes.find(name);
  if (iter == cubes.end()) {
    throw std::runtime_error("Cube " + name + " does not exist");
  }

  return iter->second;
}

}

void create_iptables_by_id(const std::string &name, const IptablesJsonObject &jsonObject) {
  {
    // check if name is valid before creating it
    std::lock_guard<std::mutex> guard(cubes_mutex);
    if (cubes.count(name) != 0) {
      throw std::runtime_error("There is already a cube with name " + name);
    }
  }
  auto ptr = std::make_shared<Iptables>(name, jsonObject);
  std::unordered_map<std::string, std::shared_ptr<Iptables>>::iterator iter;
  bool inserted;

  std::lock_guard<std::mutex> guard(cubes_mutex);
  std::tie(iter, inserted) = cubes.emplace(name, std::move(ptr));

  if (!inserted) {
    throw std::runtime_error("There is already a cube with name " + name);
  }
}

void replace_iptables_by_id(const std::string &name, const IptablesJsonObject &bridge){
  throw std::runtime_error("Method not supported!");
}

void delete_iptables_by_id(const std::string &name) {
  std::lock_guard<std::mutex> guard(cubes_mutex);
  if (cubes.count(name) == 0) {
    throw std::runtime_error("Cube " + name + " does not exist");
  }
  cubes.erase(name);
}

std::vector<IptablesJsonObject> read_iptables_list_by_id() {
  std::vector<IptablesJsonObject> jsonObject_vect;
  for(auto &i : cubes) {
    auto m = get_cube(i.first);
    jsonObject_vect.push_back(m->toJsonObject());
  }
  return jsonObject_vect;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_list_by_id_get_list() {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  for (auto &x : cubes) {
    nlohmann::fifo_map<std::string, std::string> m;
    m["name"] = x.first;
    r.push_back(std::move(m));
  }
  return r;
}

/**
* @brief   Create append by ID
*
* Create operation of resource: append*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value appendbody object
*
* Responses:
* ChainAppendOutputJsonObject
*/
ChainAppendOutputJsonObject
create_iptables_chain_append_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainAppendInputJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->append(value);

}

/**
* @brief   Create apply-rules by ID
*
* Create operation of resource: apply-rules*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* ChainApplyRulesOutputJsonObject
*/
ChainApplyRulesOutputJsonObject
create_iptables_chain_apply_rules_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->applyRules();

}

/**
* @brief   Create chain by ID
*
* Create operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
create_iptables_chain_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainJsonObject &value) {
  auto iptables = get_cube(name);

  return iptables->addChain(chainName, value);
}

/**
* @brief   Create insert by ID
*
* Create operation of resource: insert*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value insertbody object
*
* Responses:
* ChainInsertOutputJsonObject
*/
ChainInsertOutputJsonObject
create_iptables_chain_insert_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainInsertInputJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->insert(value);

}

/**
* @brief   Create chain by ID
*
* Create operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
create_iptables_chain_list_by_id(const std::string &name, const std::vector<ChainJsonObject> &value) {
  auto iptables = get_cube(name);
  iptables->addChainList(value);
}

/**
* @brief   Create remove by ID
*
* Create operation of resource: remove*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value removebody object
*
* Responses:
*
*/
void
create_iptables_chain_remove_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainRemoveInputJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->remove(value);
}

/**
* @brief   Create reset-counters by ID
*
* Create operation of resource: reset-counters*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* ChainResetCountersOutputJsonObject
*/
ChainResetCountersOutputJsonObject
create_iptables_chain_reset_counters_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->resetCounters();

}

/**
* @brief   Create rule by ID
*
* Create operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value rulebody object
*
* Responses:
*
*/
void
create_iptables_chain_rule_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const ChainRuleJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->addRule(id, value);
}

/**
* @brief   Create rule by ID
*
* Create operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value rulebody object
*
* Responses:
*
*/
void
create_iptables_chain_rule_list_by_id(const std::string &name, const ChainNameEnum &chainName, const std::vector<ChainRuleJsonObject> &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  chain->addRuleList(value);
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
create_iptables_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto iptables = get_cube(name);

  return iptables->addPorts(portsName, value);
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
create_iptables_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  auto iptables = get_cube(name);
  iptables->addPortsList(value);
}

/**
* @brief   Delete chain by ID
*
* Delete operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
*
*/
void
delete_iptables_chain_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);

  return iptables->delChain(chainName);
}

/**
* @brief   Delete chain by ID
*
* Delete operation of resource: chain*
*
* @param[in] name ID of name
*
* Responses:
*
*/
void
delete_iptables_chain_list_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  iptables->delChainList();
}

/**
* @brief   Delete rule by ID
*
* Delete operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
*
*/
void
delete_iptables_chain_rule_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->delRule(id);
}

/**
* @brief   Delete rule by ID
*
* Delete operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
*
*/
void
delete_iptables_chain_rule_list_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  chain->delRuleList();
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
delete_iptables_ports_by_id(const std::string &name, const std::string &portsName) {
  auto iptables = get_cube(name);

  return iptables->delPorts(portsName);
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
delete_iptables_ports_list_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  iptables->delPortsList();
}

/**
* @brief   Read iptables by ID
*
* Read operation of resource: iptables*
*
* @param[in] name ID of name
*
* Responses:
* IptablesJsonObject
*/
IptablesJsonObject
read_iptables_by_id(const std::string &name) {
  return get_cube(name)->toJsonObject();

}

/**
* @brief   Read chain by ID
*
* Read operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* ChainJsonObject
*/
ChainJsonObject
read_iptables_chain_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  return iptables->getChain(chainName)->toJsonObject();

}

/**
* @brief   Read default by ID
*
* Read operation of resource: default*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* ActionEnum
*/
ActionEnum
read_iptables_chain_default_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->getDefault();

}

/**
* @brief   Read chain by ID
*
* Read operation of resource: chain*
*
* @param[in] name ID of name
*
* Responses:
* std::vector<ChainJsonObject>
*/
std::vector<ChainJsonObject>
read_iptables_chain_list_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  auto &&chain = iptables->getChainList();
  std::vector<ChainJsonObject> m;
  for(auto &i : chain)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read action by ID
*
* Read operation of resource: action*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* ActionEnum
*/
ActionEnum
read_iptables_chain_rule_action_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getAction();

}

/**
* @brief   Read rule by ID
*
* Read operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* ChainRuleJsonObject
*/
ChainRuleJsonObject
read_iptables_chain_rule_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->getRule(id)->toJsonObject();

}

/**
* @brief   Read conntrack by ID
*
* Read operation of resource: conntrack*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* ConntrackstatusEnum
*/
ConntrackstatusEnum
read_iptables_chain_rule_conntrack_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getConntrack();

}

/**
* @brief   Read dport by ID
*
* Read operation of resource: dport*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_dport_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getDport();

}

/**
* @brief   Read dst by ID
*
* Read operation of resource: dst*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_dst_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getDst();

}

/**
* @brief   Read in-iface by ID
*
* Read operation of resource: in-iface*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_in_iface_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getInIface();

}

/**
* @brief   Read l4proto by ID
*
* Read operation of resource: l4proto*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_l4proto_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getL4proto();

}

/**
* @brief   Read rule by ID
*
* Read operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* std::vector<ChainRuleJsonObject>
*/
std::vector<ChainRuleJsonObject>
read_iptables_chain_rule_list_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto &&rule = chain->getRuleList();
  std::vector<ChainRuleJsonObject> m;
  for(auto &i : rule)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read out-iface by ID
*
* Read operation of resource: out-iface*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_out_iface_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getOutIface();

}

/**
* @brief   Read sport by ID
*
* Read operation of resource: sport*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_sport_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getSport();

}

/**
* @brief   Read src by ID
*
* Read operation of resource: src*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_src_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getSrc();

}

/**
* @brief   Read tcpflags by ID
*
* Read operation of resource: tcpflags*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_rule_tcpflags_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);
  return rule->getTcpflags();

}

/**
* @brief   Read stats by ID
*
* Read operation of resource: stats*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* ChainStatsJsonObject
*/
ChainStatsJsonObject
read_iptables_chain_stats_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  return chain->getStats(id)->toJsonObject();

}

/**
* @brief   Read bytes by ID
*
* Read operation of resource: bytes*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* uint64_t
*/
uint64_t
read_iptables_chain_stats_bytes_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto stats = chain->getStats(id);
  return stats->getBytes();

}

/**
* @brief   Read description by ID
*
* Read operation of resource: description*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* std::string
*/
std::string
read_iptables_chain_stats_description_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto stats = chain->getStats(id);
  return stats->getDescription();

}

/**
* @brief   Read stats by ID
*
* Read operation of resource: stats*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
*
* Responses:
* std::vector<ChainStatsJsonObject>
*/
std::vector<ChainStatsJsonObject>
read_iptables_chain_stats_list_by_id(const std::string &name, const ChainNameEnum &chainName) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto &&stats = chain->getStatsList();
  std::vector<ChainStatsJsonObject> m;
  for(auto &i : stats)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read pkts by ID
*
* Read operation of resource: pkts*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
*
* Responses:
* uint64_t
*/
uint64_t
read_iptables_chain_stats_pkts_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto stats = chain->getStats(id);
  return stats->getPkts();

}

/**
* @brief   Read conntrack by ID
*
* Read operation of resource: conntrack*
*
* @param[in] name ID of name
*
* Responses:
* IptablesConntrackEnum
*/
IptablesConntrackEnum
read_iptables_conntrack_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  return iptables->getConntrack();

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
read_iptables_dyn_opt_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  return iptables->getDynOpt();

}

/**
* @brief   Read horus by ID
*
* Read operation of resource: horus*
*
* @param[in] name ID of name
*
* Responses:
* IptablesHorusEnum
*/
IptablesHorusEnum
read_iptables_horus_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  return iptables->getHorus();

}

/**
* @brief   Read interactive by ID
*
* Read operation of resource: interactive*
*
* @param[in] name ID of name
*
* Responses:
* bool
*/
bool
read_iptables_interactive_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  return iptables->getInteractive();

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
read_iptables_ports_by_id(const std::string &name, const std::string &portsName) {
  auto iptables = get_cube(name);
  return iptables->getPorts(portsName)->toJsonObject();

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
read_iptables_ports_list_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  auto &&ports = iptables->getPortsList();
  std::vector<PortsJsonObject> m;
  for(auto &i : ports)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read session-table by ID
*
* Read operation of resource: session-table*
*
* @param[in] name ID of name
* @param[in] src ID of src
* @param[in] dst ID of dst
* @param[in] l4proto ID of l4proto
* @param[in] sport ID of sport
* @param[in] dport ID of dport
*
* Responses:
* SessionTableJsonObject
*/
SessionTableJsonObject
read_iptables_session_table_by_id(const std::string &name, const std::string &src, const std::string &dst, const std::string &l4proto, const uint16_t &sport, const uint16_t &dport) {
  auto iptables = get_cube(name);
  return iptables->getSessionTable(src, dst, l4proto, sport, dport)->toJsonObject();

}

/**
* @brief   Read session-table by ID
*
* Read operation of resource: session-table*
*
* @param[in] name ID of name
*
* Responses:
* std::vector<SessionTableJsonObject>
*/
std::vector<SessionTableJsonObject>
read_iptables_session_table_list_by_id(const std::string &name) {
  auto iptables = get_cube(name);
  auto &&sessionTable = iptables->getSessionTableList();
  std::vector<SessionTableJsonObject> m;
  for(auto &i : sessionTable)
    m.push_back(i->toJsonObject());
  return m;
}

/**
* @brief   Read state by ID
*
* Read operation of resource: state*
*
* @param[in] name ID of name
* @param[in] src ID of src
* @param[in] dst ID of dst
* @param[in] l4proto ID of l4proto
* @param[in] sport ID of sport
* @param[in] dport ID of dport
*
* Responses:
* std::string
*/
std::string
read_iptables_session_table_state_by_id(const std::string &name, const std::string &src, const std::string &dst, const std::string &l4proto, const uint16_t &sport, const uint16_t &dport) {
  auto iptables = get_cube(name);
  auto sessionTable = iptables->getSessionTable(src, dst, l4proto, sport, dport);
  return sessionTable->getState();

}

/**
* @brief   Replace chain by ID
*
* Replace operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
replace_iptables_chain_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainJsonObject &value) {
  auto iptables = get_cube(name);

  return iptables->replaceChain(chainName, value);
}

/**
* @brief   Replace chain by ID
*
* Replace operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
replace_iptables_chain_list_by_id(const std::string &name, const std::vector<ChainJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Replace rule by ID
*
* Replace operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value rulebody object
*
* Responses:
*
*/
void
replace_iptables_chain_rule_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const ChainRuleJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->replaceRule(id, value);
}

/**
* @brief   Replace rule by ID
*
* Replace operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value rulebody object
*
* Responses:
*
*/
void
replace_iptables_chain_rule_list_by_id(const std::string &name, const ChainNameEnum &chainName, const std::vector<ChainRuleJsonObject> &value) {
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
replace_iptables_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto iptables = get_cube(name);

  return iptables->replacePorts(portsName, value);
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
replace_iptables_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update iptables by ID
*
* Update operation of resource: iptables*
*
* @param[in] name ID of name
* @param[in] value iptablesbody object
*
* Responses:
*
*/
void
update_iptables_by_id(const std::string &name, const IptablesJsonObject &value) {
  auto iptables = get_cube(name);

  return iptables->update(value);
}

/**
* @brief   Update chain by ID
*
* Update operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
update_iptables_chain_by_id(const std::string &name, const ChainNameEnum &chainName, const ChainJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->update(value);
}

/**
* @brief   Update default by ID
*
* Update operation of resource: default*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value Default action if no rule matches in the ingress chain. Default is DROP.
*
* Responses:
*
*/
void
update_iptables_chain_default_by_id(const std::string &name, const ChainNameEnum &chainName, const ActionEnum &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);

  return chain->setDefault(value);
}

/**
* @brief   Update chain by ID
*
* Update operation of resource: chain*
*
* @param[in] name ID of name
* @param[in] value chainbody object
*
* Responses:
*
*/
void
update_iptables_chain_list_by_id(const std::string &name, const std::vector<ChainJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update action by ID
*
* Update operation of resource: action*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Action if the rule matches. Default is DROP.
*
* Responses:
*
*/
void
update_iptables_chain_rule_action_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const ActionEnum &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setAction(value);
}

/**
* @brief   Update rule by ID
*
* Update operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value rulebody object
*
* Responses:
*
*/
void
update_iptables_chain_rule_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const ChainRuleJsonObject &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->update(value);
}

/**
* @brief   Update conntrack by ID
*
* Update operation of resource: conntrack*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Connection status (NEW, ESTABLISHED, RELATED, INVALID)
*
* Responses:
*
*/
void
update_iptables_chain_rule_conntrack_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const ConntrackstatusEnum &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setConntrack(value);
}

/**
* @brief   Update dport by ID
*
* Update operation of resource: dport*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Destination L4 Port
*
* Responses:
*
*/
void
update_iptables_chain_rule_dport_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setDport(value);
}

/**
* @brief   Update dst by ID
*
* Update operation of resource: dst*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Destination IP Address.
*
* Responses:
*
*/
void
update_iptables_chain_rule_dst_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setDst(value);
}

/**
* @brief   Update in-iface by ID
*
* Update operation of resource: in-iface*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Name of the interface via which the packet is received
*
* Responses:
*
*/
void
update_iptables_chain_rule_in_iface_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setInIface(value);
}

/**
* @brief   Update l4proto by ID
*
* Update operation of resource: l4proto*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Level 4 Protocol.
*
* Responses:
*
*/
void
update_iptables_chain_rule_l4proto_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setL4proto(value);
}

/**
* @brief   Update rule by ID
*
* Update operation of resource: rule*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] value rulebody object
*
* Responses:
*
*/
void
update_iptables_chain_rule_list_by_id(const std::string &name, const ChainNameEnum &chainName, const std::vector<ChainRuleJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}

/**
* @brief   Update out-iface by ID
*
* Update operation of resource: out-iface*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Name of the interface via which the packet is going to be sent
*
* Responses:
*
*/
void
update_iptables_chain_rule_out_iface_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setOutIface(value);
}

/**
* @brief   Update sport by ID
*
* Update operation of resource: sport*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Source L4 Port
*
* Responses:
*
*/
void
update_iptables_chain_rule_sport_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setSport(value);
}

/**
* @brief   Update src by ID
*
* Update operation of resource: src*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value Source IP Address.
*
* Responses:
*
*/
void
update_iptables_chain_rule_src_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setSrc(value);
}

/**
* @brief   Update tcpflags by ID
*
* Update operation of resource: tcpflags*
*
* @param[in] name ID of name
* @param[in] chainName ID of chain_name
* @param[in] id ID of id
* @param[in] value TCP flags. Allowed values: SYN, FIN, ACK, RST, PSH, URG, CWR, ECE. ! means set to 0.
*
* Responses:
*
*/
void
update_iptables_chain_rule_tcpflags_by_id(const std::string &name, const ChainNameEnum &chainName, const uint32_t &id, const std::string &value) {
  auto iptables = get_cube(name);
  auto chain = iptables->getChain(chainName);
  auto rule = chain->getRule(id);

  return rule->setTcpflags(value);
}

/**
* @brief   Update conntrack by ID
*
* Update operation of resource: conntrack*
*
* @param[in] name ID of name
* @param[in] value Enables the Connection Tracking module. Mandatory if connection tracking rules are needed. Default is ON.
*
* Responses:
*
*/
void
update_iptables_conntrack_by_id(const std::string &name, const IptablesConntrackEnum &value) {
  auto iptables = get_cube(name);

  return iptables->setConntrack(value);
}

/**
* @brief   Update horus by ID
*
* Update operation of resource: horus*
*
* @param[in] name ID of name
* @param[in] value Enables the HORUS optimization. Default is OFF.
*
* Responses:
*
*/
void
update_iptables_horus_by_id(const std::string &name, const IptablesHorusEnum &value) {
  auto iptables = get_cube(name);

  return iptables->setHorus(value);
}

/**
* @brief   Update interactive by ID
*
* Update operation of resource: interactive*
*
* @param[in] name ID of name
* @param[in] value Interactive mode applies new rules immediately; if &#39;false&#39;, the command &#39;apply-rules&#39; has to be used to apply all the rules at once. Default is TRUE.
*
* Responses:
*
*/
void
update_iptables_interactive_by_id(const std::string &name, const bool &value) {
  auto iptables = get_cube(name);

  return iptables->setInteractive(value);
}

/**
* @brief   Update iptables by ID
*
* Update operation of resource: iptables*
*
* @param[in] value iptablesbody object
*
* Responses:
*
*/
void
update_iptables_list_by_id(const std::vector<IptablesJsonObject> &value) {
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
update_iptables_ports_by_id(const std::string &name, const std::string &portsName, const PortsJsonObject &value) {
  auto iptables = get_cube(name);
  auto ports = iptables->getPorts(portsName);

  return ports->update(value);
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
update_iptables_ports_list_by_id(const std::string &name, const std::vector<PortsJsonObject> &value) {
  throw std::runtime_error("Method not supported");
}



/*
 * help related
 */

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_chain_list_by_id_get_list(const std::string &name) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&iptables = get_cube(name);

  auto &&chain = iptables->getChainList();
  for(auto &i : chain) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["name"] = ChainJsonObject::ChainNameEnum_to_string(i->getName());

    r.push_back(keys);
  }
  return r;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_chain_rule_list_by_id_get_list(const std::string &name, const ChainNameEnum &chainName) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&iptables = get_cube(name);
  auto &&chain = iptables->getChain(chainName);

  auto &&rule = chain->getRuleList();
  for(auto &i : rule) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["id"] = std::to_string(i->getId());

    r.push_back(keys);
  }
  return r;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_chain_stats_list_by_id_get_list(const std::string &name, const ChainNameEnum &chainName) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&iptables = get_cube(name);
  auto &&chain = iptables->getChain(chainName);

  auto &&stats = chain->getStatsList();
  for(auto &i : stats) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["id"] = std::to_string(i->getId());

    r.push_back(keys);
  }
  return r;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_ports_list_by_id_get_list(const std::string &name) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&iptables = get_cube(name);

  auto &&ports = iptables->getPortsList();
  for(auto &i : ports) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["name"] = i->getName();

    r.push_back(keys);
  }
  return r;
}

std::vector<nlohmann::fifo_map<std::string, std::string>> read_iptables_session_table_list_by_id_get_list(const std::string &name) {
  std::vector<nlohmann::fifo_map<std::string, std::string>> r;
  auto &&iptables = get_cube(name);

  auto &&sessionTable = iptables->getSessionTableList();
  for(auto &i : sessionTable) {
    nlohmann::fifo_map<std::string, std::string> keys;

    keys["src"] = i->getSrc();
    keys["dst"] = i->getDst();
    keys["l4proto"] = i->getL4proto();
    keys["sport"] = std::to_string(i->getSport());
    keys["dport"] = std::to_string(i->getDport());

    r.push_back(keys);
  }
  return r;
}


}

}
}
}

