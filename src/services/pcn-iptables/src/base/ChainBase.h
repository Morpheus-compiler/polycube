/**
* iptables API generated from iptables.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* ChainBase.h
*
*
*/

#pragma once

#include "../serializer/ChainJsonObject.h"
#include "../serializer/ChainAppendOutputJsonObject.h"
#include "../serializer/ChainAppendInputJsonObject.h"
#include "../serializer/ChainInsertOutputJsonObject.h"
#include "../serializer/ChainInsertInputJsonObject.h"

#include "../serializer/ChainRemoveInputJsonObject.h"
#include "../serializer/ChainResetCountersOutputJsonObject.h"

#include "../serializer/ChainApplyRulesOutputJsonObject.h"


#include "../ChainRule.h"
#include "../ChainStats.h"





#include <spdlog/spdlog.h>

using namespace polycube::service::model;

class Iptables;

class ChainBase {
 public:
  
  ChainBase(Iptables &parent);
  
  virtual ~ChainBase();
  virtual void update(const ChainJsonObject &conf);
  virtual ChainJsonObject toJsonObject();

  /// <summary>
  /// Chain in which the rule will be inserted. Default: FORWARD.
  /// </summary>
  virtual ChainNameEnum getName() = 0;

  /// <summary>
  /// Default action if no rule matches in the ingress chain. Default is DROP.
  /// </summary>
  virtual ActionEnum getDefault() = 0;
  virtual void setDefault(const ActionEnum &value) = 0;

  /// <summary>
  ///
  /// </summary>
  virtual std::shared_ptr<ChainStats> getStats(const uint32_t &id) = 0;
  virtual std::vector<std::shared_ptr<ChainStats>> getStatsList() = 0;
  virtual void addStats(const uint32_t &id, const ChainStatsJsonObject &conf) = 0;
  virtual void addStatsList(const std::vector<ChainStatsJsonObject> &conf);
  virtual void replaceStats(const uint32_t &id, const ChainStatsJsonObject &conf);
  virtual void delStats(const uint32_t &id) = 0;
  virtual void delStatsList();

  /// <summary>
  ///
  /// </summary>
  virtual std::shared_ptr<ChainRule> getRule(const uint32_t &id) = 0;
  virtual std::vector<std::shared_ptr<ChainRule>> getRuleList() = 0;
  virtual void addRule(const uint32_t &id, const ChainRuleJsonObject &conf) = 0;
  virtual void addRuleList(const std::vector<ChainRuleJsonObject> &conf);
  virtual void replaceRule(const uint32_t &id, const ChainRuleJsonObject &conf);
  virtual void delRule(const uint32_t &id) = 0;
  virtual void delRuleList();
  virtual ChainAppendOutputJsonObject append(ChainAppendInputJsonObject input) = 0;
  virtual ChainInsertOutputJsonObject insert(ChainInsertInputJsonObject input) = 0;
  virtual void remove(ChainRemoveInputJsonObject input) = 0;
  virtual ChainResetCountersOutputJsonObject resetCounters() = 0;
  virtual ChainApplyRulesOutputJsonObject applyRules() = 0;

  std::shared_ptr<spdlog::logger> logger();
 protected:
  Iptables &parent_;
};
