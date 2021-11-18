/**
* iptables API generated from iptables.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "ChainRuleJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

ChainRuleJsonObject::ChainRuleJsonObject() {
  m_idIsSet = false;
  m_inIfaceIsSet = false;
  m_outIfaceIsSet = false;
  m_srcIsSet = false;
  m_dstIsSet = false;
  m_l4protoIsSet = false;
  m_sportIsSet = false;
  m_dportIsSet = false;
  m_tcpflagsIsSet = false;
  m_conntrackIsSet = false;
  m_actionIsSet = false;
}

ChainRuleJsonObject::ChainRuleJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_idIsSet = false;
  m_inIfaceIsSet = false;
  m_outIfaceIsSet = false;
  m_srcIsSet = false;
  m_dstIsSet = false;
  m_l4protoIsSet = false;
  m_sportIsSet = false;
  m_dportIsSet = false;
  m_tcpflagsIsSet = false;
  m_conntrackIsSet = false;
  m_actionIsSet = false;


  if (val.count("id")) {
    setId(val.at("id").get<uint32_t>());
  }

  if (val.count("in-iface")) {
    setInIface(val.at("in-iface").get<std::string>());
  }

  if (val.count("out-iface")) {
    setOutIface(val.at("out-iface").get<std::string>());
  }

  if (val.count("src")) {
    setSrc(val.at("src").get<std::string>());
  }

  if (val.count("dst")) {
    setDst(val.at("dst").get<std::string>());
  }

  if (val.count("l4proto")) {
    setL4proto(val.at("l4proto").get<std::string>());
  }

  if (val.count("sport")) {
    setSport(val.at("sport").get<std::string>());
  }

  if (val.count("dport")) {
    setDport(val.at("dport").get<std::string>());
  }

  if (val.count("tcpflags")) {
    setTcpflags(val.at("tcpflags").get<std::string>());
  }

  if (val.count("conntrack")) {
    setConntrack(string_to_ConntrackstatusEnum(val.at("conntrack").get<std::string>()));
  }

  if (val.count("action")) {
    setAction(string_to_ActionEnum(val.at("action").get<std::string>()));
  }
}

nlohmann::json ChainRuleJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
  }

  if (m_idIsSet) {
    val["id"] = m_id;
  }

  if (m_inIfaceIsSet) {
    val["in-iface"] = m_inIface;
  }

  if (m_outIfaceIsSet) {
    val["out-iface"] = m_outIface;
  }

  if (m_srcIsSet) {
    val["src"] = m_src;
  }

  if (m_dstIsSet) {
    val["dst"] = m_dst;
  }

  if (m_l4protoIsSet) {
    val["l4proto"] = m_l4proto;
  }

  if (m_sportIsSet) {
    val["sport"] = m_sport;
  }

  if (m_dportIsSet) {
    val["dport"] = m_dport;
  }

  if (m_tcpflagsIsSet) {
    val["tcpflags"] = m_tcpflags;
  }

  if (m_conntrackIsSet) {
    val["conntrack"] = ConntrackstatusEnum_to_string(m_conntrack);
  }

  if (m_actionIsSet) {
    val["action"] = ActionEnum_to_string(m_action);
  }

  return val;
}

uint32_t ChainRuleJsonObject::getId() const {
  return m_id;
}

void ChainRuleJsonObject::setId(uint32_t value) {
  m_id = value;
  m_idIsSet = true;
}

bool ChainRuleJsonObject::idIsSet() const {
  return m_idIsSet;
}



std::string ChainRuleJsonObject::getInIface() const {
  return m_inIface;
}

void ChainRuleJsonObject::setInIface(std::string value) {
  m_inIface = value;
  m_inIfaceIsSet = true;
}

bool ChainRuleJsonObject::inIfaceIsSet() const {
  return m_inIfaceIsSet;
}

void ChainRuleJsonObject::unsetInIface() {
  m_inIfaceIsSet = false;
}

std::string ChainRuleJsonObject::getOutIface() const {
  return m_outIface;
}

void ChainRuleJsonObject::setOutIface(std::string value) {
  m_outIface = value;
  m_outIfaceIsSet = true;
}

bool ChainRuleJsonObject::outIfaceIsSet() const {
  return m_outIfaceIsSet;
}

void ChainRuleJsonObject::unsetOutIface() {
  m_outIfaceIsSet = false;
}

std::string ChainRuleJsonObject::getSrc() const {
  return m_src;
}

void ChainRuleJsonObject::setSrc(std::string value) {
  m_src = value;
  m_srcIsSet = true;
}

bool ChainRuleJsonObject::srcIsSet() const {
  return m_srcIsSet;
}

void ChainRuleJsonObject::unsetSrc() {
  m_srcIsSet = false;
}

std::string ChainRuleJsonObject::getDst() const {
  return m_dst;
}

void ChainRuleJsonObject::setDst(std::string value) {
  m_dst = value;
  m_dstIsSet = true;
}

bool ChainRuleJsonObject::dstIsSet() const {
  return m_dstIsSet;
}

void ChainRuleJsonObject::unsetDst() {
  m_dstIsSet = false;
}

std::string ChainRuleJsonObject::getL4proto() const {
  return m_l4proto;
}

void ChainRuleJsonObject::setL4proto(std::string value) {
  m_l4proto = value;
  m_l4protoIsSet = true;
}

bool ChainRuleJsonObject::l4protoIsSet() const {
  return m_l4protoIsSet;
}

void ChainRuleJsonObject::unsetL4proto() {
  m_l4protoIsSet = false;
}

std::string ChainRuleJsonObject::getSport() const {
  return m_sport;
}

void ChainRuleJsonObject::setSport(std::string value) {
  m_sport = value;
  m_sportIsSet = true;
}

bool ChainRuleJsonObject::sportIsSet() const {
  return m_sportIsSet;
}

void ChainRuleJsonObject::unsetSport() {
  m_sportIsSet = false;
}

std::string ChainRuleJsonObject::getDport() const {
  return m_dport;
}

void ChainRuleJsonObject::setDport(std::string value) {
  m_dport = value;
  m_dportIsSet = true;
}

bool ChainRuleJsonObject::dportIsSet() const {
  return m_dportIsSet;
}

void ChainRuleJsonObject::unsetDport() {
  m_dportIsSet = false;
}

std::string ChainRuleJsonObject::getTcpflags() const {
  return m_tcpflags;
}

void ChainRuleJsonObject::setTcpflags(std::string value) {
  m_tcpflags = value;
  m_tcpflagsIsSet = true;
}

bool ChainRuleJsonObject::tcpflagsIsSet() const {
  return m_tcpflagsIsSet;
}

void ChainRuleJsonObject::unsetTcpflags() {
  m_tcpflagsIsSet = false;
}

ConntrackstatusEnum ChainRuleJsonObject::getConntrack() const {
  return m_conntrack;
}

void ChainRuleJsonObject::setConntrack(ConntrackstatusEnum value) {
  m_conntrack = value;
  m_conntrackIsSet = true;
}

bool ChainRuleJsonObject::conntrackIsSet() const {
  return m_conntrackIsSet;
}

void ChainRuleJsonObject::unsetConntrack() {
  m_conntrackIsSet = false;
}

std::string ChainRuleJsonObject::ConntrackstatusEnum_to_string(const ConntrackstatusEnum &value){
  switch(value) {
    case ConntrackstatusEnum::NEW:
      return std::string("new");
    case ConntrackstatusEnum::ESTABLISHED:
      return std::string("established");
    case ConntrackstatusEnum::RELATED:
      return std::string("related");
    case ConntrackstatusEnum::INVALID:
      return std::string("invalid");
    default:
      throw std::runtime_error("Bad ChainRule conntrack");
  }
}

ConntrackstatusEnum ChainRuleJsonObject::string_to_ConntrackstatusEnum(const std::string &str){
  if (JsonObjectBase::iequals("new", str))
    return ConntrackstatusEnum::NEW;
  if (JsonObjectBase::iequals("established", str))
    return ConntrackstatusEnum::ESTABLISHED;
  if (JsonObjectBase::iequals("related", str))
    return ConntrackstatusEnum::RELATED;
  if (JsonObjectBase::iequals("invalid", str))
    return ConntrackstatusEnum::INVALID;
  throw std::runtime_error("ChainRule conntrack is invalid");
}
ActionEnum ChainRuleJsonObject::getAction() const {
  return m_action;
}

void ChainRuleJsonObject::setAction(ActionEnum value) {
  m_action = value;
  m_actionIsSet = true;
}

bool ChainRuleJsonObject::actionIsSet() const {
  return m_actionIsSet;
}

void ChainRuleJsonObject::unsetAction() {
  m_actionIsSet = false;
}

std::string ChainRuleJsonObject::ActionEnum_to_string(const ActionEnum &value){
  switch(value) {
    case ActionEnum::DROP:
      return std::string("drop");
    case ActionEnum::LOG:
      return std::string("log");
    case ActionEnum::ACCEPT:
      return std::string("accept");
    default:
      throw std::runtime_error("Bad ChainRule action");
  }
}

ActionEnum ChainRuleJsonObject::string_to_ActionEnum(const std::string &str){
  if (JsonObjectBase::iequals("drop", str))
    return ActionEnum::DROP;
  if (JsonObjectBase::iequals("log", str))
    return ActionEnum::LOG;
  if (JsonObjectBase::iequals("accept", str))
    return ActionEnum::ACCEPT;
  throw std::runtime_error("ChainRule action is invalid");
}

}
}
}

