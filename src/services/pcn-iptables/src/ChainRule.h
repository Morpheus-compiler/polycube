/*
 * Copyright 2018 The Polycube Authors
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
#pragma once

#include "../base/ChainRuleBase.h"
#include "defines.h"

#include <inttypes.h>
#include <spdlog/spdlog.h>

class Chain;

using namespace polycube::service::model;

class ChainRule : public ChainRuleBase {
  friend class ChainStats;
  friend class Chain;

 public:
  ChainRule(Chain &parent, const ChainRuleJsonObject &conf);
  virtual ~ChainRule();

  bool equal(ChainRule &cmp);

  void update(const ChainRuleJsonObject &conf) override;
  ChainRuleJsonObject toJsonObject() override;

  /// <summary>
  /// Source IP Address.
  /// </summary>
  std::string getSrc() override;
  void setSrc(const std::string &value) override;

  /// <summary>
  /// Destination IP Address.
  /// </summary>
  std::string getDst() override;
  void setDst(const std::string &value) override;

  /// <summary>
  /// Name of the interface via which the packet is going to be sent
  /// </summary>
  std::string getOutIface() override;
  void setOutIface(const std::string &value) override;

  /// <summary>
  /// Destination L4 Port
  /// </summary>
  std::string getDport() override;
  void setDport(const std::string &value) override;

  /// <summary>
  /// TCP flags. Allowed values: SYN, FIN, ACK, RST, PSH, URG, CWR, ECE. ! means
  /// set to 0.
  /// </summary>
  std::string getTcpflags() override;
  void setTcpflags(const std::string &value) override;

  /// <summary>
  /// Name of the interface via which the packet is received
  /// </summary>
  std::string getInIface() override;
  void setInIface(const std::string &value) override;

  /// <summary>
  /// Level 4 Protocol.
  /// </summary>
  std::string getL4proto() override;
  void setL4proto(const std::string &value) override;

  /// <summary>
  /// Connection status (NEW, ESTABLISHED, RELATED, INVALID)
  /// </summary>
  ConntrackstatusEnum getConntrack() override;
  void setConntrack(const ConntrackstatusEnum &value) override;

  /// <summary>
  /// Action if the rule matches. Default is DROP.
  /// </summary>
  ActionEnum getAction() override;
  void setAction(const ActionEnum &value) override;

  /// <summary>
  /// Source L4 Port
  /// </summary>
  std::string getSport() override;
  void setSport(const std::string &value) override;

  /// <summary>
  /// Rule Identifier
  /// </summary>
  uint32_t getId() override;

  static int protocolFromStringToInt(const std::string &proto);
  static std::string protocolFromIntToString(const int &proto);
  static void flagsFromStringToMasks(const std::string &flags,
                                     uint8_t &flags_set,
                                     uint8_t &flags_not_set);
  static void flagsFromMasksToString(std::string &flags,
                                     const uint8_t &flags_set,
                                     const uint8_t &flags_not_set);

  static int ActionEnumToInt(const ActionEnum &action);
  static bool acceptEstablishedOptimizationFound(Chain &chain);
  static void applyAcceptEstablishedOptimization(Chain &chain);

  bool isSportRange();
  bool isDportRange();

  uint16_t getSportStart();
  uint16_t getSportEnd();

  uint16_t getDportStart();
  uint16_t getDportEnd();

 private:
  uint32_t id;

  ConntrackstatusEnum conntrack;
  bool conntrackIsSet = false;

  struct IpAddr ipSrc;
  bool ipSrcIsSet = false;

  struct IpAddr ipDst;
  bool ipDstIsSet = false;

  uint16_t srcPort;
  uint16_t srcPortEnd;
  bool srcPortIsSet = false;
  bool srcPortEndIsSet = false;

  uint16_t dstPort;
  uint16_t dstPortEnd;
  bool dstPortIsSet = false;
  bool dstPortEndIsSet = false;

  std::string inIface;
  bool inIfaceIsSet = false;

  std::string outIface;
  bool outIfaceIsSet = false;

  int l4Proto;
  bool l4ProtoIsSet = false;

  uint8_t flagsSet = 0, flagsNotSet = 0;
  bool tcpFlagsIsSet = false;

  ActionEnum action;
  bool actionIsSet = false;

  template <class Container>
  static void splitString(const std::string& str, Container& cont, char delim = ' ');
};
