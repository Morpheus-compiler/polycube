/*
 * Copyright 2019 The Polycube Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Transparenthelloworld.h"

// files generated by cmake from "Helloworld_dp_{ingress, egress}.c
#include "TransparentHelloworld_dp_egress.h"
#include "TransparentHelloworld_dp_ingress.h"

Transparenthelloworld::Transparenthelloworld(
    const std::string name, const TransparenthelloworldJsonObject &conf)
    : TransparentCube(conf.getBase(), {transparent_helloworld_code_ingress},
                      {transparent_helloworld_code_egress}) {
  logger()->set_pattern(
      "[%Y-%m-%d %H:%M:%S.%e] [Transparenthelloworld] [%n] [%^%l%$] %v");
  logger()->info("Creating Transparenthelloworld instance");

  setIngressAction(conf.getIngressAction());
  setEgressAction(conf.getEgressAction());
}

Transparenthelloworld::~Transparenthelloworld() {}

void Transparenthelloworld::update(
    const TransparenthelloworldJsonObject &conf) {
  TransparentCube::set_conf(conf.getBase());

  if (conf.ingressActionIsSet()) {
    setIngressAction(conf.getIngressAction());
  }

  if (conf.egressActionIsSet()) {
    setEgressAction(conf.getEgressAction());
  }
}

TransparenthelloworldJsonObject Transparenthelloworld::toJsonObject() {
  TransparenthelloworldJsonObject conf;
  conf.setBase(TransparentCube::to_json());

  conf.setIngressAction(getIngressAction());
  conf.setEgressAction(getEgressAction());

  return conf;
}

void Transparenthelloworld::packet_in(polycube::service::Direction direction,
                                      polycube::service::PacketInMetadata &md,
                                      const std::vector<uint8_t> &packet) {
  switch (direction) {
  case polycube::service::Direction::INGRESS:
    logger()->info("packet in event from ingress program");
    break;
  case polycube::service::Direction::EGRESS:
    logger()->info("packet in event from egress program");
    break;
  }
  EthernetII pkt(&packet[0], packet.size());
  send_packet_out(pkt, direction, false);
}

void Transparenthelloworld::attach() {
  logger()->info("attached");
  try {
    std::string parent_peer = get_parent_parameter("peer");
    logger()->info("parent peer is: {}", parent_peer);
  } catch (const std::exception &e) {
    logger()->warn("Error getting parent parameter: {}", e.what());
  }
}

TransparenthelloworldIngressActionEnum
Transparenthelloworld::getIngressAction() {
  auto t = get_array_table<uint8_t>("action_map", 0, ProgramType::INGRESS);
  uint8_t value = t.get(0x0);
  return static_cast<TransparenthelloworldIngressActionEnum>(value);
}

void Transparenthelloworld::setIngressAction(
    const TransparenthelloworldIngressActionEnum &value) {
  uint8_t action = static_cast<uint8_t>(value);
  auto t = get_array_table<uint8_t>("action_map", 0, ProgramType::INGRESS);
  t.set(0x0, action);
}

TransparenthelloworldEgressActionEnum Transparenthelloworld::getEgressAction() {
  auto t = get_array_table<uint8_t>("action_map", 0, ProgramType::EGRESS);
  uint8_t value = t.get(0x0);
  return static_cast<TransparenthelloworldEgressActionEnum>(value);
}

void Transparenthelloworld::setEgressAction(
    const TransparenthelloworldEgressActionEnum &value) {
  uint8_t action = static_cast<uint8_t>(value);
  auto t = get_array_table<uint8_t>("action_map", 0, ProgramType::EGRESS);
  t.set(0x0, action);
}
