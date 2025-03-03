/**
* bmc API generated from bmc.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "BmcJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

BmcJsonObject::BmcJsonObject() {
  m_nameIsSet = false;
  m_dynOpt = false;
  m_dynOptIsSet = true;
  m_portsIsSet = false;
  m_bmcStatsIsSet = false;
  m_memcachedIsSet = false;
}

BmcJsonObject::BmcJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_nameIsSet = false;
  m_dynOptIsSet = false;
  m_portsIsSet = false;
  m_bmcStatsIsSet = false;
  m_memcachedIsSet = false;


  if (val.count("name")) {
    setName(val.at("name").get<std::string>());
  }

  if (val.count("dyn-opt")) {
    setDynOpt(val.at("dyn-opt").get<bool>());
  }

  if (val.count("ports")) {
    for (auto& item : val["ports"]) {
      PortsJsonObject newItem{ item };
      m_ports.push_back(newItem);
    }

    m_portsIsSet = true;
  }

  if (val.count("bmcStats")) {
    if (!val["bmcStats"].is_null()) {
      BmcStatsJsonObject newItem { val["bmcStats"] };
      setBmcStats(newItem);
    }
  }

  if (val.count("memcached")) {
    for (auto& item : val["memcached"]) {
      MemcachedJsonObject newItem{ item };
      m_memcached.push_back(newItem);
    }

    m_memcachedIsSet = true;
  }
}

nlohmann::json BmcJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
  }

  if (m_nameIsSet) {
    val["name"] = m_name;
  }

  if (m_dynOptIsSet) {
    val["dyn-opt"] = m_dynOpt;
  }

  {
    nlohmann::json jsonArray;
    for (auto& item : m_ports) {
      jsonArray.push_back(JsonObjectBase::toJson(item));
    }

    if (jsonArray.size() > 0) {
      val["ports"] = jsonArray;
    }
  }

  if (m_bmcStatsIsSet) {
    val["bmcStats"] = JsonObjectBase::toJson(m_bmcStats);
  }

  {
    nlohmann::json jsonArray;
    for (auto& item : m_memcached) {
      jsonArray.push_back(JsonObjectBase::toJson(item));
    }

    if (jsonArray.size() > 0) {
      val["memcached"] = jsonArray;
    }
  }

  return val;
}

std::string BmcJsonObject::getName() const {
  return m_name;
}

void BmcJsonObject::setName(std::string value) {
  m_name = value;
  m_nameIsSet = true;
}

bool BmcJsonObject::nameIsSet() const {
  return m_nameIsSet;
}



bool BmcJsonObject::getDynOpt() const {
  return m_dynOpt;
}

void BmcJsonObject::setDynOpt(bool value) {
  m_dynOpt = value;
  m_dynOptIsSet = true;
}

bool BmcJsonObject::dynOptIsSet() const {
  return m_dynOptIsSet;
}

void BmcJsonObject::unsetDynOpt() {
  m_dynOptIsSet = false;
}

const std::vector<PortsJsonObject>& BmcJsonObject::getPorts() const{
  return m_ports;
}

void BmcJsonObject::addPorts(PortsJsonObject value) {
  m_ports.push_back(value);
  m_portsIsSet = true;
}


bool BmcJsonObject::portsIsSet() const {
  return m_portsIsSet;
}

void BmcJsonObject::unsetPorts() {
  m_portsIsSet = false;
}

BmcStatsJsonObject BmcJsonObject::getBmcStats() const {
  return m_bmcStats;
}

void BmcJsonObject::setBmcStats(BmcStatsJsonObject value) {
  m_bmcStats = value;
  m_bmcStatsIsSet = true;
}

bool BmcJsonObject::bmcStatsIsSet() const {
  return m_bmcStatsIsSet;
}

void BmcJsonObject::unsetBmcStats() {
  m_bmcStatsIsSet = false;
}

const std::vector<MemcachedJsonObject>& BmcJsonObject::getMemcached() const{
  return m_memcached;
}

void BmcJsonObject::addMemcached(MemcachedJsonObject value) {
  m_memcached.push_back(value);
  m_memcachedIsSet = true;
}


bool BmcJsonObject::memcachedIsSet() const {
  return m_memcachedIsSet;
}

void BmcJsonObject::unsetMemcached() {
  m_memcachedIsSet = false;
}


}
}
}

