/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "GeneralStatsGetLruMissStatsOutputJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

GeneralStatsGetLruMissStatsOutputJsonObject::GeneralStatsGetLruMissStatsOutputJsonObject() {
  m_v1IsSet = false;
  m_v2IsSet = false;
}

GeneralStatsGetLruMissStatsOutputJsonObject::GeneralStatsGetLruMissStatsOutputJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_v1IsSet = false;
  m_v2IsSet = false;


  if (val.count("v1")) {
    setV1(val.at("v1").get<uint64_t>());
  }

  if (val.count("v2")) {
    setV2(val.at("v2").get<uint64_t>());
  }
}

nlohmann::json GeneralStatsGetLruMissStatsOutputJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
  }

  if (m_v1IsSet) {
    val["v1"] = m_v1;
  }

  if (m_v2IsSet) {
    val["v2"] = m_v2;
  }

  return val;
}

uint64_t GeneralStatsGetLruMissStatsOutputJsonObject::getV1() const {
  return m_v1;
}

void GeneralStatsGetLruMissStatsOutputJsonObject::setV1(uint64_t value) {
  m_v1 = value;
  m_v1IsSet = true;
}

bool GeneralStatsGetLruMissStatsOutputJsonObject::v1IsSet() const {
  return m_v1IsSet;
}

void GeneralStatsGetLruMissStatsOutputJsonObject::unsetV1() {
  m_v1IsSet = false;
}

uint64_t GeneralStatsGetLruMissStatsOutputJsonObject::getV2() const {
  return m_v2;
}

void GeneralStatsGetLruMissStatsOutputJsonObject::setV2(uint64_t value) {
  m_v2 = value;
  m_v2IsSet = true;
}

bool GeneralStatsGetLruMissStatsOutputJsonObject::v2IsSet() const {
  return m_v2IsSet;
}

void GeneralStatsGetLruMissStatsOutputJsonObject::unsetV2() {
  m_v2IsSet = false;
}


}
}
}

