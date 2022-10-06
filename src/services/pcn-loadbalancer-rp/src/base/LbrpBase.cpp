/**
* lbrp API generated from lbrp.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "LbrpBase.h"

LbrpBase::LbrpBase(const std::string name) {
  logger()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [Lbrp] [%n] [%^%l%$] %v");
}



LbrpBase::~LbrpBase() {}

void LbrpBase::update(const LbrpJsonObject &conf) {
  set_conf(conf.getBase());

  if (conf.startMorpheusIsSet()) {
    setStartMorpheus(conf.getStartMorpheus());
  }
  if (conf.portsIsSet()) {
    for (auto &i : conf.getPorts()) {
      auto name = i.getName();
      auto m = getPorts(name);
      m->update(i);
    }
  }
  if (conf.portModeIsSet()) {
    setPortMode(conf.getPortMode());
  }
  if (conf.srcIpRewriteIsSet()) {
    auto m = getSrcIpRewrite();
    m->update(conf.getSrcIpRewrite());
  }
  if (conf.serviceIsSet()) {
    for (auto &i : conf.getService()) {
      auto vip = i.getVip();
      auto vport = i.getVport();
      auto proto = i.getProto();
      auto m = getService(vip, vport, proto);
      m->update(i);
    }
  }
}

LbrpJsonObject LbrpBase::toJsonObject() {
  LbrpJsonObject conf;
  conf.setBase(to_json());

  conf.setName(getName());
  conf.setDynOpt(getDynOpt());
  conf.setStartMorpheus(getStartMorpheus());
  for (auto &i : getPortsList()) {
    conf.addPorts(i->toJsonObject());
  }
  conf.setPortMode(getPortMode());
  conf.setSrcIpRewrite(getSrcIpRewrite()->toJsonObject());
  for(auto &i : getServiceList()) {
    conf.addService(i->toJsonObject());
  }

  return conf;
}
void LbrpBase::addPortsList(const std::vector<PortsJsonObject> &conf) {
  for (auto &i : conf) {
    std::string name_ = i.getName();
    addPorts(name_, i);
  }
}

void LbrpBase::replacePorts(const std::string &name, const PortsJsonObject &conf) {
  delPorts(name);
  std::string name_ = conf.getName();
  addPorts(name_, conf);
}

void LbrpBase::delPortsList() {
  auto elements = getPortsList();
  for (auto &i : elements) {
    std::string name_ = i->getName();
    delPorts(name_);
  }
}

void LbrpBase::addPorts(const std::string &name, const PortsJsonObject &conf) {
  add_port<PortsJsonObject>(name, conf);
}

void LbrpBase::delPorts(const std::string &name) {
  remove_port(name);
}

std::shared_ptr<Ports> LbrpBase::getPorts(const std::string &name) {
  return get_port(name);
}

std::vector<std::shared_ptr<Ports>> LbrpBase::getPortsList() {
  return get_ports();
}

void LbrpBase::replaceSrcIpRewrite(const SrcIpRewriteJsonObject &conf) {
  // TODO: This is a basic default implementation, maybe you want to improve it
  delSrcIpRewrite();
  addSrcIpRewrite(conf);
}
void LbrpBase::addServiceList(const std::vector<ServiceJsonObject> &conf) {
  for (auto &i : conf) {
    std::string vip_ = i.getVip();
    uint16_t vport_ = i.getVport();
    ServiceProtoEnum proto_ = i.getProto();
    addService(vip_, vport_, proto_, i);
  }
}

void LbrpBase::replaceService(const std::string &vip, const uint16_t &vport, const ServiceProtoEnum &proto, const ServiceJsonObject &conf) {
  delService(vip, vport, proto);
  std::string vip_ = conf.getVip();
  uint16_t vport_ = conf.getVport();
  ServiceProtoEnum proto_ = conf.getProto();
  addService(vip_, vport_, proto_, conf);
}

void LbrpBase::delServiceList() {
  auto elements = getServiceList();
  for (auto &i : elements) {
    std::string vip_ = i->getVip();
    uint16_t vport_ = i->getVport();
    ServiceProtoEnum proto_ = i->getProto();
    delService(vip_, vport_, proto_);
  }
}


