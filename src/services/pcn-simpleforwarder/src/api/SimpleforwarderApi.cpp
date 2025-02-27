/**
* simpleforwarder API generated from simpleforwarder.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "SimpleforwarderApi.h"
#include "SimpleforwarderApiImpl.h"

using namespace polycube::service::model;
using namespace polycube::service::api::SimpleforwarderApiImpl;

#ifdef __cplusplus
extern "C" {
#endif

Response create_simpleforwarder_actions_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    ActionsJsonObject unique_value { request_body };

    unique_value.setInport(unique_inport);
    create_simpleforwarder_actions_by_id(unique_name, unique_inport, unique_value);
    return { kCreated, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response create_simpleforwarder_actions_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<ActionsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<ActionsJsonObject> unique_value;
    for (auto &j : request_body) {
      ActionsJsonObject a { j };
      unique_value.push_back(a);
    }
    create_simpleforwarder_actions_list_by_id(unique_name, unique_value);
    return { kCreated, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response create_simpleforwarder_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    SimpleforwarderJsonObject unique_value { request_body };

    unique_value.setName(unique_name);
    create_simpleforwarder_by_id(unique_name, unique_value);
    return { kCreated, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response create_simpleforwarder_ports_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    PortsJsonObject unique_value { request_body };

    unique_value.setName(unique_portsName);
    create_simpleforwarder_ports_by_id(unique_name, unique_portsName, unique_value);
    return { kCreated, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response create_simpleforwarder_ports_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<PortsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<PortsJsonObject> unique_value;
    for (auto &j : request_body) {
      PortsJsonObject a { j };
      unique_value.push_back(a);
    }
    create_simpleforwarder_ports_list_by_id(unique_name, unique_value);
    return { kCreated, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response delete_simpleforwarder_actions_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    delete_simpleforwarder_actions_by_id(unique_name, unique_inport);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response delete_simpleforwarder_actions_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {
    delete_simpleforwarder_actions_list_by_id(unique_name);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response delete_simpleforwarder_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {
    delete_simpleforwarder_by_id(unique_name);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response delete_simpleforwarder_ports_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    delete_simpleforwarder_ports_by_id(unique_name, unique_portsName);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response delete_simpleforwarder_ports_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {
    delete_simpleforwarder_ports_list_by_id(unique_name);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_actions_action_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {

    auto x = read_simpleforwarder_actions_action_by_id(unique_name, unique_inport);
    nlohmann::json response_body;
    response_body = ActionsJsonObject::ActionsActionEnum_to_string(x);
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_actions_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {

    auto x = read_simpleforwarder_actions_by_id(unique_name, unique_inport);
    nlohmann::json response_body;
    response_body = x.toJson();
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_actions_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {

    auto x = read_simpleforwarder_actions_list_by_id(unique_name);
    nlohmann::json response_body;
    for (auto &i : x) {
      response_body += i.toJson();
    }
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_actions_outport_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {

    auto x = read_simpleforwarder_actions_outport_by_id(unique_name, unique_inport);
    nlohmann::json response_body;
    response_body = x;
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {

    auto x = read_simpleforwarder_by_id(unique_name);
    nlohmann::json response_body;
    response_body = x.toJson();
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_dyn_opt_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {

    auto x = read_simpleforwarder_dyn_opt_by_id(unique_name);
    nlohmann::json response_body;
    response_body = x;
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {


  try {

    auto x = read_simpleforwarder_list_by_id();
    nlohmann::json response_body;
    for (auto &i : x) {
      response_body += i.toJson();
    }
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_ports_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {

    auto x = read_simpleforwarder_ports_by_id(unique_name, unique_portsName);
    nlohmann::json response_body;
    response_body = x.toJson();
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_ports_ip_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {

    auto x = read_simpleforwarder_ports_ip_by_id(unique_name, unique_portsName);
    nlohmann::json response_body;
    response_body = x;
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_ports_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {

    auto x = read_simpleforwarder_ports_list_by_id(unique_name);
    nlohmann::json response_body;
    for (auto &i : x) {
      response_body += i.toJson();
    }
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response read_simpleforwarder_simple_redirect_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ) {
  // Getting the path params
  std::string unique_name { name };

  try {

    auto x = read_simpleforwarder_simple_redirect_by_id(unique_name);
    nlohmann::json response_body;
    response_body = x;
    return { kOk, ::strdup(response_body.dump().c_str()) };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response replace_simpleforwarder_actions_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    ActionsJsonObject unique_value { request_body };

    unique_value.setInport(unique_inport);
    replace_simpleforwarder_actions_by_id(unique_name, unique_inport, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response replace_simpleforwarder_actions_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<ActionsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<ActionsJsonObject> unique_value;
    for (auto &j : request_body) {
      ActionsJsonObject a { j };
      unique_value.push_back(a);
    }
    replace_simpleforwarder_actions_list_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response replace_simpleforwarder_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    SimpleforwarderJsonObject unique_value { request_body };

    unique_value.setName(unique_name);
    replace_simpleforwarder_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response replace_simpleforwarder_ports_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    PortsJsonObject unique_value { request_body };

    unique_value.setName(unique_portsName);
    replace_simpleforwarder_ports_by_id(unique_name, unique_portsName, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response replace_simpleforwarder_ports_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<PortsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<PortsJsonObject> unique_value;
    for (auto &j : request_body) {
      PortsJsonObject a { j };
      unique_value.push_back(a);
    }
    replace_simpleforwarder_ports_list_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_actions_action_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    ActionsActionEnum unique_value_ = ActionsJsonObject::string_to_ActionsActionEnum(request_body);
    update_simpleforwarder_actions_action_by_id(unique_name, unique_inport, unique_value_);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_actions_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    ActionsJsonObject unique_value { request_body };

    unique_value.setInport(unique_inport);
    update_simpleforwarder_actions_by_id(unique_name, unique_inport, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_actions_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<ActionsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<ActionsJsonObject> unique_value;
    for (auto &j : request_body) {
      ActionsJsonObject a { j };
      unique_value.push_back(a);
    }
    update_simpleforwarder_actions_list_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_actions_outport_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_inport;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "inport")) {
      unique_inport = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // The conversion is done automatically by the json library
    std::string unique_value = request_body;
    update_simpleforwarder_actions_outport_by_id(unique_name, unique_inport, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    SimpleforwarderJsonObject unique_value { request_body };

    unique_value.setName(unique_name);
    update_simpleforwarder_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {

  // Getting the body param
  std::vector<SimpleforwarderJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<SimpleforwarderJsonObject> unique_value;
    for (auto &j : request_body) {
      SimpleforwarderJsonObject a { j };
      unique_value.push_back(a);
    }
    update_simpleforwarder_list_by_id(unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_ports_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    PortsJsonObject unique_value { request_body };

    unique_value.setName(unique_portsName);
    update_simpleforwarder_ports_by_id(unique_name, unique_portsName, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_ports_ip_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  std::string unique_portsName;
  for (size_t i = 0; i < num_keys; ++i) {
    if (!strcmp(keys[i].name, "ports_name")) {
      unique_portsName = std::string { keys[i].value.string };
      break;
    }
  }


  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // The conversion is done automatically by the json library
    std::string unique_value = request_body;
    update_simpleforwarder_ports_ip_by_id(unique_name, unique_portsName, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_ports_list_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };
  // Getting the body param
  std::vector<PortsJsonObject> unique_value;

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // Getting the body param
    std::vector<PortsJsonObject> unique_value;
    for (auto &j : request_body) {
      PortsJsonObject a { j };
      unique_value.push_back(a);
    }
    update_simpleforwarder_ports_list_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}

Response update_simpleforwarder_simple_redirect_by_id_handler(
  const char *name, const Key *keys,
  size_t num_keys ,
  const char *value) {
  // Getting the path params
  std::string unique_name { name };

  try {
    auto request_body = nlohmann::json::parse(std::string { value });
    // The conversion is done automatically by the json library
    bool unique_value = request_body;
    update_simpleforwarder_simple_redirect_by_id(unique_name, unique_value);
    return { kOk, nullptr };
  } catch(const std::exception &e) {
    return { kGenericError, ::strdup(e.what()) };
  }
}


Response simpleforwarder_actions_list_by_id_help(
  const char *name, const Key *keys, size_t num_keys) {
  // Getting the path params
  std::string unique_name { name };
  nlohmann::json val = read_simpleforwarder_actions_list_by_id_get_list(unique_name);

  return { kOk, ::strdup(val.dump().c_str()) };
}

Response simpleforwarder_list_by_id_help(
  const char *name, const Key *keys, size_t num_keys) {

  nlohmann::json val = read_simpleforwarder_list_by_id_get_list();

  return { kOk, ::strdup(val.dump().c_str()) };
}

Response simpleforwarder_ports_list_by_id_help(
  const char *name, const Key *keys, size_t num_keys) {
  // Getting the path params
  std::string unique_name { name };
  nlohmann::json val = read_simpleforwarder_ports_list_by_id_get_list(unique_name);

  return { kOk, ::strdup(val.dump().c_str()) };
}

#ifdef __cplusplus
}
#endif

