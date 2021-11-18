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

#include "Chain.h"
#include "ChainRule.h"
#include "Iptables.h"

ChainRule::ChainRule(Chain &parent, const ChainRuleJsonObject &conf)
    : ChainRuleBase(parent), id(conf.getId()) {
  logger()->trace("Creating ChainRule instance");
  update(conf);
}

ChainRule::~ChainRule() {}

void ChainRule::update(const ChainRuleJsonObject &conf) {
  // This method updates all the object/parameter in ChainRule object specified
  // in the conf JsonObject.
  // You can modify this implementation.

  if (conf.conntrackIsSet() && parent_.parent_.isContrackActive()) {
    this->conntrack = conf.getConntrack();
    conntrackIsSet = true;
  }
  if (conf.srcIsSet()) {
    this->ipSrc.fromString(conf.getSrc());
    ipSrcIsSet = true;
  }
  if (conf.dstIsSet()) {
    this->ipDst.fromString(conf.getDst());
    ipDstIsSet = true;
  }
  if (conf.sportIsSet()) {
    if (conf.getSport().find(':') != std::string::npos) {
      std::vector<std::string> res;
      splitString(conf.getSport(), res, ':');
      if (res.size() != 2) {
        throw std::runtime_error("Source port is not correctly formatted");
      }

      unsigned int sport = std::stoul(res[0]);
      unsigned int sport_end = std::stoul(res[1]);

      if (sport > std::numeric_limits<uint16_t>::max() || sport_end > std::numeric_limits<uint16_t>::max()) {
        throw std::runtime_error("Source port is not in the correct range" + std::to_string(sport));
      } else if (sport == 0 && sport_end == 65535) {
        // Do not do nothing, it is like wildcard
        srcPortIsSet = false;
        srcPortEndIsSet = false;
      } else if (sport == sport_end) {
        this->srcPort = sport;
        srcPortIsSet = true;
        srcPortEndIsSet = false;
      } else {
        this->srcPort = sport;
        this->srcPortEnd = sport_end;
        srcPortIsSet = true;
        srcPortEndIsSet = true;
      }
    } else {
      unsigned int sport = std::stoul(conf.getSport());
      if (sport >= std::numeric_limits<uint16_t>::max()) {
        throw std::runtime_error("Source port is not in the correct range"  + std::to_string(sport));
      } else {
        this->srcPort = sport;
        srcPortIsSet = true;
        srcPortEndIsSet = false;
      }
    }
  }
  if (conf.dportIsSet()) {
    if (conf.getDport().find(':') != std::string::npos) {
      std::vector<std::string> res;
      splitString(conf.getDport(), res, ':');
      if (res.size() != 2) {
        throw std::runtime_error("Destination port is not correctly formatted");
      }

      unsigned int dport = std::stoul(res[0]);
      unsigned int dport_end = std::stoul(res[1]);

      if (dport > std::numeric_limits<uint16_t>::max() || dport_end > std::numeric_limits<uint16_t>::max()) {
        throw std::runtime_error("Destination port is not in the correct range"  + std::to_string(dport));
      } else if (dport == 0 && dport_end == 65535) {
        // Do not do nothing, it is like wildcard
        dstPortIsSet = false;
        dstPortEndIsSet = false;
      } else if (dport == dport_end) {
        this->dstPort = dport;
        dstPortIsSet = true;
        dstPortEndIsSet = false;
      } else {
        this->dstPort = dport;
        this->dstPortEnd = dport_end;
        dstPortIsSet = true;
        dstPortEndIsSet = true;
      }
    } else {
      unsigned int dport = std::stoul(conf.getDport());
      if (dport >= std::numeric_limits<uint16_t>::max()) {
        throw std::runtime_error("Destination port is not in the correct range" + std::to_string(dport));
      } else {
        this->dstPort = dport;
        dstPortIsSet = true;
        dstPortEndIsSet = false;
      }
    }
  }
  if (conf.tcpflagsIsSet()) {
    flagsFromStringToMasks(conf.getTcpflags(), flagsSet, flagsNotSet);
    tcpFlagsIsSet = true;
  }
  if (conf.l4protoIsSet()) {
    l4Proto = protocolFromStringToInt(conf.getL4proto());
    l4ProtoIsSet = true;
  }
  if (conf.inIfaceIsSet()) {
    this->inIface = conf.getInIface();
    parent_.parent_.interfaceNameToIndex(inIface);
    inIfaceIsSet = true;
  }
  if (conf.outIfaceIsSet()) {
    this->outIface = conf.getOutIface();
    parent_.parent_.interfaceNameToIndex(outIface);
    outIfaceIsSet = true;
  }
  if (conf.actionIsSet()) {
    this->action = conf.getAction();
    actionIsSet = true;
  } else {
    this->action = ActionEnum::DROP;
    actionIsSet = true;
  }
}

template <class Container>
void ChainRule::splitString(const std::string& str, Container& cont, char delim)
{
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim)) {
    cont.push_back(token);
  }
}

ChainRuleJsonObject ChainRule::toJsonObject() {
  ChainRuleJsonObject conf;

  try {
    conf.setSrc(getSrc());
  } catch (...) {
  }
  try {
    conf.setDst(getDst());
  } catch (...) {
  }
  try {
    conf.setDport(getDport());
  } catch (...) {
  }
  try {
    conf.setTcpflags(getTcpflags());
  } catch (...) {
  }
  try {
    conf.setL4proto(getL4proto());
  } catch (...) {
  }
  try {
    conf.setAction(getAction());
  } catch (...) {
  }
  try {
    conf.setSport(getSport());
  } catch (...) {
  }
  try {
    conf.setId(getId());
  } catch (...) {
  }
  try {
    conf.setOutIface(getOutIface());
  } catch (...) {
  }
  try {
    conf.setInIface(getInIface());
  } catch (...) {
  }
  try {
    conf.setConntrack(getConntrack());
  } catch (...) {
  }

  return conf;
}

bool ChainRule::equal(ChainRule &cmp) {
  if (ipSrcIsSet != cmp.ipSrcIsSet)
    return false;
  if (ipSrcIsSet) {
    if (ipSrc.toString() != cmp.ipSrc.toString())
      return false;
  }

  if (ipDstIsSet != cmp.ipDstIsSet)
    return false;
  if (ipDstIsSet) {
    if (ipDst.toString() != cmp.ipDst.toString())
      return false;
  }

  if (srcPortIsSet != cmp.srcPortIsSet)
    return false;
  if (srcPortIsSet) {
    if (srcPort != cmp.srcPort)
      return false;
  }

  if (dstPortIsSet != cmp.dstPortIsSet)
    return false;
  if (dstPortIsSet) {
    if (dstPort != cmp.dstPort)
      return false;
  }

  if (l4ProtoIsSet != cmp.l4ProtoIsSet)
    return false;
  if (l4ProtoIsSet) {
    if (l4Proto != cmp.l4Proto)
      return false;
  }

  if (tcpFlagsIsSet != cmp.tcpFlagsIsSet)
    return false;
  if (tcpFlagsIsSet) {
    if ((flagsSet != cmp.flagsSet) || ((flagsNotSet != cmp.flagsNotSet)))
      return false;
  }

  if (inIfaceIsSet != cmp.inIfaceIsSet)
    return false;
  if (inIfaceIsSet) {
    if (inIface != cmp.inIface)
      return false;
  }

  if (outIfaceIsSet != cmp.outIfaceIsSet)
    return false;
  if (outIfaceIsSet) {
    if (outIface != cmp.outIface)
      return false;
  }

  if (actionIsSet != cmp.actionIsSet)
    return false;

  if (actionIsSet) {
    if (action != cmp.action)
      return false;
  }

  if (conntrackIsSet != cmp.conntrackIsSet)
    return false;
  if (conntrackIsSet) {
    if (conntrack != cmp.conntrack)
      return false;
  }

  if (isSportRange() && cmp.isSportRange()) {
    if (srcPortEnd != cmp.srcPortEnd) {
      return false;
    }
  } else if (!isSportRange() != !cmp.isSportRange()) {
    return false;
  }

  if (isDportRange() && cmp.isDportRange()) {
    if (dstPortEnd != cmp.dstPortEnd) {
      return false;
    }
  } else if (!isDportRange() != !cmp.isDportRange()) {
    return false;
  }

  return true;
}

void ChainRule::applyAcceptEstablishedOptimization(Chain &chain) {
  if (acceptEstablishedOptimizationFound(chain))
    chain.parent_.enableAcceptEstablished(chain);
  else
    chain.parent_.disableAcceptEstablished(chain);
}

bool ChainRule::acceptEstablishedOptimizationFound(Chain &chain) {
  if (chain.rules_.size() > 0) {
    ChainRuleJsonObject conf;

    conf.setConntrack(ConntrackstatusEnum::ESTABLISHED);
    conf.setAction(ActionEnum::ACCEPT);

    ChainRule c(chain, conf);
    if (chain.rules_[0] != nullptr) {
      if (chain.rules_[0]->equal(c)) {
        chain.logger()->trace(
            "Accept established optimization founded in rule-set");
        return true;
      }
    }
  }

  chain.logger()->trace(
      "Accept established optimization NOT founded in rule-set");
  return false;
}

// Using removeEntry method is not resizing the rule set, it is just replacing
// the current rule with no-rule
// the result is

std::string ChainRule::getSrc() {
  // This method retrieves the src value.
  if (!ipSrcIsSet) {
    throw std::runtime_error("Src not set.");
  }
  return this->ipSrc.toString();
}

void ChainRule::setSrc(const std::string &value) {
  // This method set the src value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getDst() {
  // This method retrieves the dst value.
  if (!ipDstIsSet) {
    throw std::runtime_error("Dst not set.");
  }
  return this->ipDst.toString();
}

void ChainRule::setDst(const std::string &value) {
  // This method set the dst value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getOutIface() {
  // This method retrieves the outIface value.
  if (!outIfaceIsSet) {
    throw std::runtime_error("Out iface not set.");
  }
  return this->outIface;
}

void ChainRule::setOutIface(const std::string &value) {
  // This method set the outIface value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getDport() {
  // This method retrieves the dport value.
  if (!dstPortIsSet) {
    throw std::runtime_error("DPort not set.");
  } else if (dstPortEndIsSet) {
    return std::string(std::to_string(this->dstPort) + ":" + std::to_string(this->dstPortEnd));
  } else {
    return std::to_string(this->dstPort);
  }
}

void ChainRule::setDport(const std::string &value) {
  // This method set the dport value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getTcpflags() {
  // This method retrieves the tcpflags value.
  if (!tcpFlagsIsSet) {
    throw std::runtime_error("TcpFlags not set.");
  }
  std::string flags = "";
  flagsFromMasksToString(flags, this->flagsSet, this->flagsNotSet);
  return flags;
}

void ChainRule::setTcpflags(const std::string &value) {
  // This method set the tcpflags value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getInIface() {
  // This method retrieves the inIface value.
  if (!inIfaceIsSet) {
    throw std::runtime_error("Out iface not set.");
  }
  return this->inIface;
}

void ChainRule::setInIface(const std::string &value) {
  // This method set the inIface value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}
std::string ChainRule::getL4proto() {
  // This method retrieves the l4proto value.
  if (!l4ProtoIsSet) {
    throw std::runtime_error("L4Proto not set.");
  }
  return protocolFromIntToString(this->l4Proto);
}

void ChainRule::setL4proto(const std::string &value) {
  // This method set the l4proto value.
  throw std::runtime_error("[ChainRule]: Method setL4proto not implemented");
}

ConntrackstatusEnum ChainRule::getConntrack() {
  if (!conntrackIsSet) {
    throw std::runtime_error("conntrack not set.");
  }
  return this->conntrack;
}

void ChainRule::setConntrack(const ConntrackstatusEnum &value) {
  // This method set the conntrack value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

ActionEnum ChainRule::getAction() {
  // This method retrieves the action value.
  if (!actionIsSet) {
    throw std::runtime_error("Action not set.");
  }
  return this->action;
}

void ChainRule::setAction(const ActionEnum &value) {
  // This method set the action value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

std::string ChainRule::getSport() {
  // This method retrieves the sport value.
  if (!srcPortIsSet) {
    throw std::runtime_error("SPort not set.");
  } else if (srcPortEndIsSet) {
    return std::string(std::to_string(this->srcPort) + ":" + std::to_string(this->srcPortEnd));
  } else {
    return std::to_string(this->srcPort);
  }
}

void ChainRule::setSport(const std::string &value) {
  // This method set the sport value.
  throw std::runtime_error(
      "It is not possible to modify rule fields once created. Replace it with "
      "new one.");
}

// ConntrackstatusEnum ChainRule::getConntrack() {
//   if (!conntrackIsSet) {
//     throw std::runtime_error("Conntrack not set.");
//   }
//   return this->conntrack;
// }

uint32_t ChainRule::getId() {
  // This method retrieves the id value.
  return id;
}

bool ChainRule::isDportRange() {
  return dstPortEndIsSet;
}

bool ChainRule::isSportRange() {
  return srcPortEndIsSet;
}

uint16_t ChainRule::getSportStart() {
  if (!srcPortIsSet) {
    throw std::runtime_error("sport not set.");
  }

  return srcPort;
}

uint16_t ChainRule::getSportEnd() {
  if (!srcPortEndIsSet) {
    throw std::runtime_error("sport end not set.");
  }

  return srcPortEnd;
}

uint16_t ChainRule::getDportStart() {
  if (!dstPortIsSet) {
    throw std::runtime_error("dport not set.");
  }

  return dstPort;
}

uint16_t ChainRule::getDportEnd() {
  if (!dstPortEndIsSet) {
    throw std::runtime_error("dport end not set.");
  }

  return dstPortEnd;
}
