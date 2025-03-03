/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "VipRealBase.h"
#include "../Katran.h"


VipRealBase::VipRealBase(Vip &parent)
    : parent_(parent) {}

VipRealBase::~VipRealBase() {}

void VipRealBase::update(const VipRealJsonObject &conf) {

  if (conf.weightIsSet()) {
    setWeight(conf.getWeight());
  }
  if (conf.flagsIsSet()) {
    setFlags(conf.getFlags());
  }
}

VipRealJsonObject VipRealBase::toJsonObject() {
  VipRealJsonObject conf;

  conf.setIp(getIp());
  conf.setWeight(getWeight());
  conf.setFlags(getFlags());

  return conf;
}

std::shared_ptr<spdlog::logger> VipRealBase::logger() {
  return parent_.logger();
}

