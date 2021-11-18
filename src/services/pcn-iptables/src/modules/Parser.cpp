/*
 * Copyright 2017 The Polycube Authors
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

#include "../Iptables.h"
#include "datapaths/Iptables_Parser_dp.h"

Iptables::Parser::Parser(const int &index, Iptables &outer, const ProgramType t)
    : Iptables::Program(iptables_code_parser, index,
                        (t == ProgramType::INGRESS)
                            ? ChainNameEnum::INVALID_INGRESS
                            : ChainNameEnum::INVALID_EGRESS,
                        outer, t) {
  reload();
}

Iptables::Parser::~Parser() = default;

std::string Iptables::Parser::defaultActionString(ChainNameEnum chain) {
  try {
    auto target_chain = iptables_.getChain(chain);
    if (target_chain->getDefault() == ActionEnum::DROP) {
      return "return RX_DROP;";
    } else if (target_chain->getDefault() == ActionEnum::ACCEPT) {
      return "return RX_OK;";
    }
    
    throw std::runtime_error("Chain does not have default action");
  } catch (...) {
    return "return RX_DROP;";
  }
  return "";
}

void Iptables::Parser::updateDefaultActionTable(const ChainNameEnum &chainName) {
  std::string table_name;
  auto chain = iptables_.getChain(chainName);
  switch (chainName) {
    case ChainNameEnum::INPUT:
      table_name = "default_action_Input";
      break;
    case ChainNameEnum::FORWARD:
      table_name = "default_action_Forward";
      break;
    case ChainNameEnum::OUTPUT:
      table_name = "default_action_Output";
      break;
    default:
      table_name = "undefined";
      break;
  }

  auto table = iptables_.get_array_table<uint64_t>(table_name, index_, program_type_);
  uint64_t value = 0;

  if (chain->getDefault() == ActionEnum::ACCEPT) {
    value = 1;
  }

  table.set(0, value);
}

void Iptables::Parser::updateIsTableEmpty(const ChainNameEnum &chainName, const std::vector<std::shared_ptr<ChainRule>> &ruleList) {
  std::string table_name;
  auto chain = iptables_.getChain(chainName);
  switch (chainName) {
    case ChainNameEnum::INPUT:
      table_name = "input_chain_empty";
      break;
    case ChainNameEnum::FORWARD:
      table_name = "forward_chain_empty";
      break;
    case ChainNameEnum::OUTPUT:
      table_name = "output_chain_empty";
      break;
    default:
      table_name = "undefined";
      break;
  }

  auto table = iptables_.get_array_table<uint64_t>(table_name, index_, program_type_);
  table.set(0, (uint64_t)ruleList.empty());
}

std::string Iptables::Parser::getCode() {
  std::string no_macro_code = code_;

  // /*Ingress or Egress logic*/
  if (program_type_ == ProgramType::INGRESS) {
    // if this is a parser for INPUT hook:
    replaceAll(no_macro_code, "_INGRESS_LOGIC", std::to_string(1));
    replaceAll(no_macro_code, "_EGRESS_LOGIC", std::to_string(0));
  } else if (program_type_ == ProgramType::EGRESS) {
    // if this is a parser for OUTPUT hook:
    replaceAll(no_macro_code, "_INGRESS_LOGIC", std::to_string(0));
    replaceAll(no_macro_code, "_EGRESS_LOGIC", std::to_string(1));
  }

  /*Replacing the maximum number of rules*/
  replaceAll(no_macro_code, "_MAXRULES",
             std::to_string(FROM_NRULES_TO_NELEMENTS(iptables_.max_rules_)));

  /* Replacing next hops_*/
  if (program_type_ == ProgramType::INGRESS)
    replaceAll(no_macro_code, "_CHAINSELECTOR",
               std::to_string(ModulesConstants::CHAINSELECTOR_INGRESS));
  else if (program_type_ == ProgramType::EGRESS)
    replaceAll(no_macro_code, "_CHAINSELECTOR",
               std::to_string(ModulesConstants::CHAINSELECTOR_EGRESS));

  if (program_type_ == ProgramType::INGRESS) {
    replaceAll(no_macro_code, "call_bpf_program", "call_ingress_program");
  } else if (program_type_ == ProgramType::EGRESS) {
    replaceAll(no_macro_code, "call_bpf_program", "call_egress_program");
  }

  if (iptables_.horus_runtime_enabled_) {
    replaceAll(no_macro_code, "_HORUS_ENABLED", "1");
  } else {
    replaceAll(no_macro_code, "_HORUS_ENABLED", "0");
  }

  if (iptables_.horus_swap_) {
    replaceAll(no_macro_code, "_HORUS",
               std::to_string(ModulesConstants::HORUS_INGRESS_SWAP));
  } else {
    replaceAll(no_macro_code, "_HORUS",
               std::to_string(ModulesConstants::HORUS_INGRESS));
  }

  return no_macro_code;
}
