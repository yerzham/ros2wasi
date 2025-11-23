#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"


extern const char * wasm_identifier;

extern "C" {

RMW_PUBLIC
rmw_client_t *
rmw_create_client(
  const rmw_node_t * node,
  const rosidl_service_type_support_t * type_support,
  const char * service_name,
  const rmw_qos_profile_t * qos_profile)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    wasm_identifier,
    return nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(service_name, nullptr);
  if (strlen(service_name) == 0) {
    RMW_SET_ERROR_MSG("service_name is empty");
    return nullptr;
  }
  RMW_CHECK_ARGUMENT_FOR_NULL(qos_profile, nullptr);
  
  rmw_client_t * client = new rmw_client_t();
  client->implementation_identifier = wasm_identifier;
  client->data = nullptr;
  client->service_name = service_name;
  
  return client;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_client(rmw_node_t * node, rmw_client_t * client)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  delete client;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_send_request(
  const rmw_client_t * client,
  const void * ros_request,
  int64_t * sequence_id)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ros_request, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(sequence_id, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *sequence_id = 0;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_response(
  const rmw_client_t * client,
  rmw_service_info_t * request_header,
  void * ros_response,
  bool * taken)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(request_header, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ros_response, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *taken = false;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_client_request_publisher_get_actual_qos(
  const rmw_client_t * client,
  rmw_qos_profile_t * qos)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *qos = rmw_qos_profile_default;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_client_response_subscription_get_actual_qos(
  const rmw_client_t * client,
  rmw_qos_profile_t * qos)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *qos = rmw_qos_profile_default;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_client_set_on_new_response_callback(
  rmw_client_t * client,
  rmw_event_callback_t callback,
  const void * user_data)
{
  (void)callback;
  (void)user_data;
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_get_gid_for_client(const rmw_client_t * client, rmw_gid_t * gid)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(gid, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    client,
    client->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  // Stub implementation - return a dummy GID
  memset(gid->data, 0, RMW_GID_STORAGE_SIZE);
  gid->implementation_identifier = wasm_identifier;
  return RMW_RET_OK;
}

}  // extern "C"
