/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


// TODO: Modify these methods with your own implementation


#include "GeneralStats.h"
#include "Katran.h"

#include "polycube/common.h"


GeneralStats::GeneralStats(Katran &parent, const GeneralStatsJsonObject &conf)
    : GeneralStatsBase(parent) {
}

GeneralStats::~GeneralStats() {}

uint64_t GeneralStats::getV1() {
  return 0;
}

uint64_t GeneralStats::getV2() {
  return 0;
}

katran::lb_stats GeneralStats::getLbStats(uint32_t position, const std::string& map) {
  if (parent_.config_.disableForwarding) {
    logger()->error("getLbStats called on non-forwarding instance");
    return katran::lb_stats{};
  }
  unsigned int nr_cpus = polycube::get_possible_cpu_count();
  if (nr_cpus < 0) {
    logger()->error("Error while getting number of possible cpus");
    return katran::lb_stats{};
  }
  katran::lb_stats sum_stat = {};

  if (!parent_.config_.testing) {
    auto statsTable = parent_.get_percpuarray_table<katran::lb_stats>(map);
    auto statsVector = statsTable.get(position);
    for (auto& stat : statsVector) {
      sum_stat.v1 += stat.v1;
      sum_stat.v2 += stat.v2;
    }
  }
  return sum_stat;
}

uint8_t GeneralStats::vipStatsInputToProtocolNumer(const GeneralStatsGetVipStatsInputProtoEnum &proto) {
  switch (proto)
  {
  case GeneralStatsGetVipStatsInputProtoEnum::TCP:
    return 6;
  case GeneralStatsGetVipStatsInputProtoEnum::UDP:
    return 17;
  case GeneralStatsGetVipStatsInputProtoEnum::ICMP:
    return 1;
  default:
    return 255;
  }
}


GeneralStatsGetVipStatsOutputJsonObject GeneralStats::getVipStats(GeneralStatsGetVipStatsInputJsonObject input) {
  katran::VipKey vip;
  vip.address = input.getAddress();
  vip.port = input.getPort();
  vip.proto = GeneralStats::vipStatsInputToProtocolNumer(input.getProto());

  GeneralStatsGetVipStatsOutputJsonObject result;
  result.setV1(0);
  result.setV2(0);

  auto vip_iter = parent_.vips_.find(vip);
  if (vip_iter == parent_.vips_.end()) {
    logger()->error("trying to get stats for non-existing vip");
    return result;
  }
  auto num = vip_iter->second.getVipNum();
  auto finalStats = getLbStats(num);

  result.setV1(finalStats.v1);
  result.setV2(finalStats.v2);

  return result;
}

GeneralStatsGetLruStatsOutputJsonObject GeneralStats::getLruStats() {
  GeneralStatsGetLruStatsOutputJsonObject result;
  auto finalStats = getLbStats(parent_.config_.maxVips + katran::kLruCntrOffset);

  result.setV1(finalStats.v1);
  result.setV2(finalStats.v2);

  return result;
}

GeneralStatsGetLruMissStatsOutputJsonObject GeneralStats::getLruMissStats() {
  GeneralStatsGetLruMissStatsOutputJsonObject result;
  auto finalStats = getLbStats(parent_.config_.maxVips + katran::kLruMissOffset);

  result.setV1(finalStats.v1);
  result.setV2(finalStats.v2);

  return result;
}

GeneralStatsGetLruFallbackStatsOutputJsonObject GeneralStats::getLruFallbackStats() {
  GeneralStatsGetLruFallbackStatsOutputJsonObject result;
  auto finalStats = getLbStats(parent_.config_.maxVips + katran::kLruFallbackOffset);

  result.setV1(finalStats.v1);
  result.setV2(finalStats.v2);

  return result;
}

GeneralStatsGetIcmpTooBigStatsOutputJsonObject GeneralStats::getIcmpTooBigStats() {
  GeneralStatsGetIcmpTooBigStatsOutputJsonObject result;
  auto finalStats = getLbStats(parent_.config_.maxVips + katran::kIcmpTooBigOffset);

  result.setV1(finalStats.v1);
  result.setV2(finalStats.v2);

  return result;
}

