/**
* iptables API generated from iptables.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "SessionTableJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

SessionTableJsonObject::SessionTableJsonObject() {
  m_srcIsSet = false;
  m_dstIsSet = false;
  m_l4protoIsSet = false;
  m_sportIsSet = false;
  m_dportIsSet = false;
  m_stateIsSet = false;
}

SessionTableJsonObject::SessionTableJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_srcIsSet = false;
  m_dstIsSet = false;
  m_l4protoIsSet = false;
  m_sportIsSet = false;
  m_dportIsSet = false;
  m_stateIsSet = false;


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
    setSport(val.at("sport").get<uint16_t>());
  }

  if (val.count("dport")) {
    setDport(val.at("dport").get<uint16_t>());
  }

  if (val.count("state")) {
    setState(val.at("state").get<std::string>());
  }
}

nlohmann::json SessionTableJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
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

  if (m_stateIsSet) {
    val["state"] = m_state;
  }

  return val;
}

std::string SessionTableJsonObject::getSrc() const {
  return m_src;
}

void SessionTableJsonObject::setSrc(std::string value) {
  m_src = value;
  m_srcIsSet = true;
}

bool SessionTableJsonObject::srcIsSet() const {
  return m_srcIsSet;
}



std::string SessionTableJsonObject::getDst() const {
  return m_dst;
}

void SessionTableJsonObject::setDst(std::string value) {
  m_dst = value;
  m_dstIsSet = true;
}

bool SessionTableJsonObject::dstIsSet() const {
  return m_dstIsSet;
}



std::string SessionTableJsonObject::getL4proto() const {
  return m_l4proto;
}

void SessionTableJsonObject::setL4proto(std::string value) {
  m_l4proto = value;
  m_l4protoIsSet = true;
}

bool SessionTableJsonObject::l4protoIsSet() const {
  return m_l4protoIsSet;
}



uint16_t SessionTableJsonObject::getSport() const {
  return m_sport;
}

void SessionTableJsonObject::setSport(uint16_t value) {
  m_sport = value;
  m_sportIsSet = true;
}

bool SessionTableJsonObject::sportIsSet() const {
  return m_sportIsSet;
}



uint16_t SessionTableJsonObject::getDport() const {
  return m_dport;
}

void SessionTableJsonObject::setDport(uint16_t value) {
  m_dport = value;
  m_dportIsSet = true;
}

bool SessionTableJsonObject::dportIsSet() const {
  return m_dportIsSet;
}



std::string SessionTableJsonObject::getState() const {
  return m_state;
}

void SessionTableJsonObject::setState(std::string value) {
  m_state = value;
  m_stateIsSet = true;
}

bool SessionTableJsonObject::stateIsSet() const {
  return m_stateIsSet;
}

void SessionTableJsonObject::unsetState() {
  m_stateIsSet = false;
}


}
}
}

