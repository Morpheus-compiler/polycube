/**
* katran API generated from katran.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */

/*
* VipJsonObject.h
*
*
*/

#pragma once


#include "JsonObjectBase.h"

#include "VipRealJsonObject.h"
#include <vector>

namespace polycube {
namespace service {
namespace model {

enum class VipProtoEnum {
  ICMP, TCP, UDP, ALL
};

/// <summary>
///
/// </summary>
class  VipJsonObject : public JsonObjectBase {
public:
  VipJsonObject();
  VipJsonObject(const nlohmann::json &json);
  ~VipJsonObject() final = default;
  nlohmann::json toJson() const final;


  /// <summary>
  /// Virtual IP (vip) of the service where clients connect to
  /// </summary>
  std::string getAddress() const;
  void setAddress(std::string value);
  bool addressIsSet() const;

  /// <summary>
  /// Port of the virtual server where clients connect to (this value is ignored in case of ICMP)
  /// </summary>
  uint16_t getPort() const;
  void setPort(uint16_t value);
  bool portIsSet() const;

  /// <summary>
  /// Upper-layer protocol associated with a loadbalancing service instance. &#39;ALL&#39; creates an entry for all the supported protocols
  /// </summary>
  VipProtoEnum getProto() const;
  void setProto(VipProtoEnum value);
  bool protoIsSet() const;
  static std::string VipProtoEnum_to_string(const VipProtoEnum &value);
  static VipProtoEnum string_to_VipProtoEnum(const std::string &str);

  /// <summary>
  /// Flags used in the VIP
  /// </summary>
  uint32_t getFlags() const;
  void setFlags(uint32_t value);
  bool flagsIsSet() const;
  void unsetFlags();

  /// <summary>
  /// Pool of backend servers that actually serve requests
  /// </summary>
  const std::vector<VipRealJsonObject>& getReal() const;
  void addVipReal(VipRealJsonObject value);
  bool realIsSet() const;
  void unsetReal();

private:
  std::string m_address;
  bool m_addressIsSet;
  uint16_t m_port;
  bool m_portIsSet;
  VipProtoEnum m_proto;
  bool m_protoIsSet;
  uint32_t m_flags;
  bool m_flagsIsSet;
  std::vector<VipRealJsonObject> m_real;
  bool m_realIsSet;
};

}
}
}

