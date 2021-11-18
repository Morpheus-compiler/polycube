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

// Modify these methods with your own implementation

#include "Simplebridge.h"
#include "Simplebridge_dp.h"

#include <tins/ethernetII.h>
#include <tins/tins.h>
#include <thread>
#include <regex>

using namespace Tins;

Simplebridge::Simplebridge(const std::string name,
                           const SimplebridgeJsonObject &conf)
    : Cube(conf.getBase(), {simplebridge_code}, {}), SimplebridgeBase(name),
    opt_code(simplebridge_code), quit_thread_(false) {
  logger()->info("Creating Simplebridge instance");
  addPortsList(conf.getPorts());
  addFdb(conf.getFdb());

  timestamp_update_thread_ =
      std::thread(&Simplebridge::updateTimestampTimer, this);

  //optimization_thread_ =
  //    std::thread(&Simplebridge::optimizationThreadReload, this);
}

Simplebridge::~Simplebridge() {
  logger()->info("Destroying Simplebridge instance");
  // we are destroying this service, prepare for it.
  quitAndJoin();
}

void Simplebridge::quitAndJoin() {
  quit_thread_ = true;
  timestamp_update_thread_.join();
  //optimization_thread_.join();
}

void Simplebridge::updateTimestampTimer() {
  do {
    sleep(1);
    updateTimestamp();
  } while (!quit_thread_);
}

void Simplebridge::optimizationThreadReload() {
  uint8_t i = 0;
  do {
    sleep(1); i++;
    if(i >= 30) {
      i=0;
      reloadWithOptimization();
    }
  } while (!quit_thread_);
}

/*
 * This method is in charge of updating the timestamp table
 * that is used in the dataplane to avoid calling the bpf_ktime helper
 * that introduces a non-negligible overhead to the eBPF program.
 */
void Simplebridge::updateTimestamp() {
  try {
    // get timestamp from system
    struct timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    auto timestamp_table = get_array_table<uint32_t>("timestamp");
    timestamp_table.set(0, now_timespec.tv_sec);
  } catch (...) {
    logger()->error("Error while updating the timestamp table");
  }
}

void Simplebridge::packet_in(Ports &port,
                             polycube::service::PacketInMetadata &md,
                             const std::vector<uint8_t> &packet) {
  logger()->debug("Packet received from port {0}", port.name());

  try {
    // logger()->debug("[{0}] packet from port: '{1}' id: '{2}' peer:'{3}'",
    //  get_name(), port.name(), port.index(), port.peer());

    switch (static_cast<SlowPathReason>(md.reason)) {
    case SlowPathReason::FLOODING:
      logger()->debug("Received a pkt in slowpath - reason: FLOODING");
      flood_packet(port, md, packet);
      break;
    default:
      logger()->error("Not valid reason {0} received", md.reason);
    }
  } catch (const std::exception &e) {
    logger()->error("exception during slowpath packet processing: '{0}'",
                    e.what());
  }
}

void Simplebridge::flood_packet(Port &port, PacketInMetadata &md,
                                const std::vector<uint8_t> &packet) {
  EthernetII p(&packet[0], packet.size());

  for (auto &it : get_ports()) {
    if (it->name() == port.name()) {
      continue;
    }
    it->send_packet_out(p);
    logger()->trace("Packet sent to port {0} as result of flooding",
                    it->name());
  }
}

void Simplebridge::reloadCodeWithAgingtime(uint32_t aging_time) {
  logger()->debug("Reloading code with agingtime: {0}", aging_time);

  std::string aging_time_str("#define AGING_TIME " +
                             std::to_string(aging_time));

  reload(aging_time_str + opt_code);

  logger()->trace("New bridge code reloaded");
}

void Simplebridge::reloadWithOptimization() {
  logger()->info("Reading all the maps and try to JIT entries");

  std::regex re(R"(entry = fwdtable\.lookup\(&dst_mac\);)");
  std::string fwd_switch_case_code = getFwdSwitchCaseCode();

  std::string new_code = std::regex_replace(simplebridge_code, re, fwd_switch_case_code);

  if (new_code != opt_code) {
    // OK, let's reload
    reload(new_code);
    opt_code = new_code;
    logger()->info("The new code has been reloaded :)");
  } else {
    logger()->debug("All the possible optimizations have been already applied");
  }
}

std::string Simplebridge::getFwdSwitchCaseCode() {
  std::ostringstream new_lookup_code;
  int num_entries = 0;

  auto fwdtable = get_hash_table<uint64_t, fwd_entry>("fwdtable");
  auto fdb = fwdtable.get_all();

  new_lookup_code << "switch (dst_mac) {" << std::endl;
  new_lookup_code << "  default:" << std::endl;
  new_lookup_code << "    entry = fwdtable.lookup(&dst_mac);" << std::endl;
  new_lookup_code << "    dst_interface = entry->port;" << std::endl;
  new_lookup_code << "    break;" << std::endl;

  for (auto &pair : fdb) {
    auto map_key = pair.first;
    auto map_entry = pair.second;

    new_lookup_code << "  case " << std::to_string(map_key) << ":" << std::endl;
    new_lookup_code << "    dst_interface = " << std::to_string(map_entry.port) << ";" << std::endl;
    new_lookup_code << R"(  pcn_log(ctx, LOG_DEBUG, "Matched direct entry"); )" << std::endl;
    new_lookup_code << "    goto FORWARD;" << std::endl;
    if (++num_entries > 5) {
      break;
    }
  }

  new_lookup_code << "}" << std::endl;

  return new_lookup_code.str();
}

std::shared_ptr<Fdb> Simplebridge::getFdb() {
  if (fdb_ != nullptr) {
    return fdb_;
  } else {
    return std::make_shared<Fdb>(*this);
  }
}

void Simplebridge::addFdb(const FdbJsonObject &value) {
  fdb_ = std::make_shared<Fdb>(*this, value);
}

void Simplebridge::replaceFdb(const FdbJsonObject &conf) {
  SimplebridgeBase::replaceFdb(conf);
}

void Simplebridge::delFdb() {
  if (fdb_ != nullptr) {
    fdb_->delEntryList();

    // I don't want to delete the Filtering database. This is very strange
    // I'll only reset the agingTime, if needed
    fdb_->setAgingTime(300);
    // parent.fdb_.reset();
    // parent.fdb_ = nullptr;
  } else {
    // This should never happen
    throw std::runtime_error("There is not filtering database in the bridge");
  }
}
