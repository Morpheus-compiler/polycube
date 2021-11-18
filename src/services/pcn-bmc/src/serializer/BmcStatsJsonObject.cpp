/**
* bmc API generated from bmc.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */



#include "BmcStatsJsonObject.h"
#include <regex>

namespace polycube {
namespace service {
namespace model {

BmcStatsJsonObject::BmcStatsJsonObject() {
  m_getRecvCountIsSet = false;
  m_setRecvCountIsSet = false;
  m_getRespCountIsSet = false;
  m_hitMispredictionIsSet = false;
  m_hitCountIsSet = false;
  m_missCountIsSet = false;
  m_updateCountIsSet = false;
  m_invalidationCountIsSet = false;
}

BmcStatsJsonObject::BmcStatsJsonObject(const nlohmann::json &val) :
  JsonObjectBase(val) {
  m_getRecvCountIsSet = false;
  m_setRecvCountIsSet = false;
  m_getRespCountIsSet = false;
  m_hitMispredictionIsSet = false;
  m_hitCountIsSet = false;
  m_missCountIsSet = false;
  m_updateCountIsSet = false;
  m_invalidationCountIsSet = false;


  if (val.count("get_recv_count")) {
    setGetRecvCount(val.at("get_recv_count").get<uint32_t>());
  }

  if (val.count("set_recv_count")) {
    setSetRecvCount(val.at("set_recv_count").get<uint32_t>());
  }

  if (val.count("get_resp_count")) {
    setGetRespCount(val.at("get_resp_count").get<uint32_t>());
  }

  if (val.count("hit_misprediction")) {
    setHitMisprediction(val.at("hit_misprediction").get<uint32_t>());
  }

  if (val.count("hit_count")) {
    setHitCount(val.at("hit_count").get<uint32_t>());
  }

  if (val.count("miss_count")) {
    setMissCount(val.at("miss_count").get<uint32_t>());
  }

  if (val.count("update_count")) {
    setUpdateCount(val.at("update_count").get<uint32_t>());
  }

  if (val.count("invalidation_count")) {
    setInvalidationCount(val.at("invalidation_count").get<uint32_t>());
  }
}

nlohmann::json BmcStatsJsonObject::toJson() const {
  nlohmann::json val = nlohmann::json::object();
  if (!getBase().is_null()) {
    val.update(getBase());
  }

  if (m_getRecvCountIsSet) {
    val["get_recv_count"] = m_getRecvCount;
  }

  if (m_setRecvCountIsSet) {
    val["set_recv_count"] = m_setRecvCount;
  }

  if (m_getRespCountIsSet) {
    val["get_resp_count"] = m_getRespCount;
  }

  if (m_hitMispredictionIsSet) {
    val["hit_misprediction"] = m_hitMisprediction;
  }

  if (m_hitCountIsSet) {
    val["hit_count"] = m_hitCount;
  }

  if (m_missCountIsSet) {
    val["miss_count"] = m_missCount;
  }

  if (m_updateCountIsSet) {
    val["update_count"] = m_updateCount;
  }

  if (m_invalidationCountIsSet) {
    val["invalidation_count"] = m_invalidationCount;
  }

  return val;
}

uint32_t BmcStatsJsonObject::getGetRecvCount() const {
  return m_getRecvCount;
}

void BmcStatsJsonObject::setGetRecvCount(uint32_t value) {
  m_getRecvCount = value;
  m_getRecvCountIsSet = true;
}

bool BmcStatsJsonObject::getRecvCountIsSet() const {
  return m_getRecvCountIsSet;
}

void BmcStatsJsonObject::unsetGetRecvCount() {
  m_getRecvCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getSetRecvCount() const {
  return m_setRecvCount;
}

void BmcStatsJsonObject::setSetRecvCount(uint32_t value) {
  m_setRecvCount = value;
  m_setRecvCountIsSet = true;
}

bool BmcStatsJsonObject::setRecvCountIsSet() const {
  return m_setRecvCountIsSet;
}

void BmcStatsJsonObject::unsetSetRecvCount() {
  m_setRecvCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getGetRespCount() const {
  return m_getRespCount;
}

void BmcStatsJsonObject::setGetRespCount(uint32_t value) {
  m_getRespCount = value;
  m_getRespCountIsSet = true;
}

bool BmcStatsJsonObject::getRespCountIsSet() const {
  return m_getRespCountIsSet;
}

void BmcStatsJsonObject::unsetGetRespCount() {
  m_getRespCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getHitMisprediction() const {
  return m_hitMisprediction;
}

void BmcStatsJsonObject::setHitMisprediction(uint32_t value) {
  m_hitMisprediction = value;
  m_hitMispredictionIsSet = true;
}

bool BmcStatsJsonObject::hitMispredictionIsSet() const {
  return m_hitMispredictionIsSet;
}

void BmcStatsJsonObject::unsetHitMisprediction() {
  m_hitMispredictionIsSet = false;
}

uint32_t BmcStatsJsonObject::getHitCount() const {
  return m_hitCount;
}

void BmcStatsJsonObject::setHitCount(uint32_t value) {
  m_hitCount = value;
  m_hitCountIsSet = true;
}

bool BmcStatsJsonObject::hitCountIsSet() const {
  return m_hitCountIsSet;
}

void BmcStatsJsonObject::unsetHitCount() {
  m_hitCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getMissCount() const {
  return m_missCount;
}

void BmcStatsJsonObject::setMissCount(uint32_t value) {
  m_missCount = value;
  m_missCountIsSet = true;
}

bool BmcStatsJsonObject::missCountIsSet() const {
  return m_missCountIsSet;
}

void BmcStatsJsonObject::unsetMissCount() {
  m_missCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getUpdateCount() const {
  return m_updateCount;
}

void BmcStatsJsonObject::setUpdateCount(uint32_t value) {
  m_updateCount = value;
  m_updateCountIsSet = true;
}

bool BmcStatsJsonObject::updateCountIsSet() const {
  return m_updateCountIsSet;
}

void BmcStatsJsonObject::unsetUpdateCount() {
  m_updateCountIsSet = false;
}

uint32_t BmcStatsJsonObject::getInvalidationCount() const {
  return m_invalidationCount;
}

void BmcStatsJsonObject::setInvalidationCount(uint32_t value) {
  m_invalidationCount = value;
  m_invalidationCountIsSet = true;
}

bool BmcStatsJsonObject::invalidationCountIsSet() const {
  return m_invalidationCountIsSet;
}

void BmcStatsJsonObject::unsetInvalidationCount() {
  m_invalidationCountIsSet = false;
}


}
}
}

