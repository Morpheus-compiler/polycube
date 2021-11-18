/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* GeneralStatsGetVipStatsOutputJsonObject.h
*
*
*/

#pragma once


#include "JsonObjectBase.h"


namespace polycube {
namespace service {
namespace model {


/// <summary>
///
/// </summary>
class  GeneralStatsGetVipStatsOutputJsonObject : public JsonObjectBase {
public:
  GeneralStatsGetVipStatsOutputJsonObject();
  GeneralStatsGetVipStatsOutputJsonObject(const nlohmann::json &json);
  ~GeneralStatsGetVipStatsOutputJsonObject() final = default;
  nlohmann::json toJson() const final;


  /// <summary>
  /// V1 Stats
  /// </summary>
  uint64_t getV1() const;
  void setV1(uint64_t value);
  bool v1IsSet() const;
  void unsetV1();

  /// <summary>
  /// V2 Stats
  /// </summary>
  uint64_t getV2() const;
  void setV2(uint64_t value);
  bool v2IsSet() const;
  void unsetV2();

private:
  uint64_t m_v1;
  bool m_v1IsSet;
  uint64_t m_v2;
  bool m_v2IsSet;
};

}
}
}

