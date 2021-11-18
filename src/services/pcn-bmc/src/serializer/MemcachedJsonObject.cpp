/**
* bmc API generated from bmc.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "MemcachedJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

MemcachedJsonObject::MemcachedJsonObject() {
  m_hashKeyIsSet = false;
  m_lenIsSet = false;
  m_validIsSet = false;
  m_hashIsSet = false;
  m_dataIsSet = false;
}

MemcachedJsonObject::MemcachedJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_hashKeyIsSet = false;
  m_lenIsSet = false;
  m_validIsSet = false;
  m_hashIsSet = false;
  m_dataIsSet = false;


  if (val.count("hash-key")) {
    setHashKey(val.at("hash-key").get<uint32_t>());
  }

  if (val.count("len")) {
    setLen(val.at("len").get<uint32_t>());
  }

  if (val.count("valid")) {
    setValid(val.at("valid").get<std::string>());
  }

  if (val.count("hash")) {
    setHash(val.at("hash").get<int32_t>());
  }

  if (val.count("data")) {
    setData(val.at("data").get<std::string>());
  }
}

nlohmann::json MemcachedJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
  }

  if (m_hashKeyIsSet) {
    val["hash-key"] = m_hashKey;
  }

  if (m_lenIsSet) {
    val["len"] = m_len;
  }

  if (m_validIsSet) {
    val["valid"] = m_valid;
  }

  if (m_hashIsSet) {
    val["hash"] = m_hash;
  }

  if (m_dataIsSet) {
    val["data"] = m_data;
  }

  return val;
}

uint32_t MemcachedJsonObject::getHashKey() const {
  return m_hashKey;
}

void MemcachedJsonObject::setHashKey(uint32_t value) {
  m_hashKey = value;
  m_hashKeyIsSet = true;
}

bool MemcachedJsonObject::hashKeyIsSet() const {
  return m_hashKeyIsSet;
}



uint32_t MemcachedJsonObject::getLen() const {
  return m_len;
}

void MemcachedJsonObject::setLen(uint32_t value) {
  m_len = value;
  m_lenIsSet = true;
}

bool MemcachedJsonObject::lenIsSet() const {
  return m_lenIsSet;
}

void MemcachedJsonObject::unsetLen() {
  m_lenIsSet = false;
}

std::string MemcachedJsonObject::getValid() const {
  return m_valid;
}

void MemcachedJsonObject::setValid(std::string value) {
  m_valid = value;
  m_validIsSet = true;
}

bool MemcachedJsonObject::validIsSet() const {
  return m_validIsSet;
}

void MemcachedJsonObject::unsetValid() {
  m_validIsSet = false;
}

int32_t MemcachedJsonObject::getHash() const {
  return m_hash;
}

void MemcachedJsonObject::setHash(int32_t value) {
  m_hash = value;
  m_hashIsSet = true;
}

bool MemcachedJsonObject::hashIsSet() const {
  return m_hashIsSet;
}

void MemcachedJsonObject::unsetHash() {
  m_hashIsSet = false;
}

std::string MemcachedJsonObject::getData() const {
  return m_data;
}

void MemcachedJsonObject::setData(std::string value) {
  m_data = value;
  m_dataIsSet = true;
}

bool MemcachedJsonObject::dataIsSet() const {
  return m_dataIsSet;
}

void MemcachedJsonObject::unsetData() {
  m_dataIsSet = false;
}


}
}
}

