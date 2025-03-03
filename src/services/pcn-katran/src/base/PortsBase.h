/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* PortsBase.h
*
*
*/

#pragma once

#include "../serializer/PortsJsonObject.h"




#include "polycube/services/cube.h"
#include "polycube/services/port.h"

#include "polycube/services/utils.h"
#include "polycube/services/fifo_map.hpp"

#include <spdlog/spdlog.h>

using namespace polycube::service::model;

class Katran;
class Ports;

class PortsBase: public polycube::service::Port {
 public:
  PortsBase(polycube::service::Cube<Ports> &parent,
      std::shared_ptr<polycube::service::PortIface> port);
  
  virtual ~PortsBase();
  virtual void update(const PortsJsonObject &conf);
  virtual PortsJsonObject toJsonObject();

  std::shared_ptr<spdlog::logger> logger();
 protected:
  Katran &parent_;
};
