#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/get_network_flow_endpoints.h"
#include <cstring>
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"

extern "C" {

RMW_PUBLIC
rmw_publisher_t *
rmw_create_publisher(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_profile,
  const rmw_publisher_options_t * publisher_options)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, nullptr);
  if (strlen(topic_name) == 0) {
    RMW_SET_ERROR_MSG("topic_name is empty");
    return nullptr;
  }
  RMW_CHECK_ARGUMENT_FOR_NULL(qos_profile, nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher_options, nullptr);
  
  rmw_publisher_t * publisher = new rmw_publisher_t();
  publisher->implementation_identifier = rmw_wasm_component_cpp::identifier;
  publisher->data = nullptr;
  publisher->topic_name = topic_name;
  publisher->can_loan_messages = false;
  
  return publisher;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_publisher(rmw_node_t * node, rmw_publisher_t * publisher)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  delete publisher;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publish(
  const rmw_publisher_t * publisher,
  const void * ros_message,
  rmw_publisher_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publish_serialized_message(
  const rmw_publisher_t * publisher,
  const rmw_serialized_message_t * serialized_message,
  rmw_publisher_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publisher_count_matched_subscriptions(
  const rmw_publisher_t * publisher,
  size_t * subscription_count)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription_count, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *subscription_count = 0;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publisher_assert_liveliness(const rmw_publisher_t * publisher)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publisher_wait_for_all_acked(
  const rmw_publisher_t * publisher,
  rmw_time_t wait_timeout)
{
  (void)wait_timeout;
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_publisher_get_actual_qos(
  const rmw_publisher_t * publisher,
  rmw_qos_profile_t * qos)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *qos = rmw_qos_profile_default;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_borrow_loaned_message(
  const rmw_publisher_t * publisher,
  const rosidl_message_type_support_t * type_support,
  void ** ros_message)
{
  (void)publisher;
  (void)type_support;
  (void)ros_message;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_return_loaned_message_from_publisher(
  const rmw_publisher_t * publisher,
  void * loaned_message)
{
  (void)publisher;
  (void)loaned_message;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_publish_loaned_message(
  const rmw_publisher_t * publisher,
  void * ros_message,
  rmw_publisher_allocation_t * allocation)
{
  (void)publisher;
  (void)ros_message;
  (void)allocation;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_publisher_get_network_flow_endpoints(
  const rmw_publisher_t * publisher,
  rcutils_allocator_t * allocator,
  rmw_network_flow_endpoint_array_t * network_flow_endpoint_array)
{
  (void)publisher;
  (void)allocator;
  RMW_CHECK_ARGUMENT_FOR_NULL(network_flow_endpoint_array, RMW_RET_INVALID_ARGUMENT);
  
  network_flow_endpoint_array->size = 0;
  network_flow_endpoint_array->network_flow_endpoint = nullptr;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_get_network_flow_endpoints(
  const rmw_subscription_t * subscription,
  rcutils_allocator_t * allocator,
  rmw_network_flow_endpoint_array_t * network_flow_endpoint_array)
{
  (void)subscription;
  (void)allocator;
  RMW_CHECK_ARGUMENT_FOR_NULL(network_flow_endpoint_array, RMW_RET_INVALID_ARGUMENT);
  
  network_flow_endpoint_array->size = 0;
  network_flow_endpoint_array->network_flow_endpoint = nullptr;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_get_gid_for_publisher(const rmw_publisher_t * publisher, rmw_gid_t * gid)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(gid, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  // Stub implementation - return a dummy GID
  memset(gid->data, 0, RMW_GID_STORAGE_SIZE);
  gid->implementation_identifier = rmw_wasm_component_cpp::identifier;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_compare_gids_equal(const rmw_gid_t * gid1, const rmw_gid_t * gid2, bool * result)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(gid1, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(gid2, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(result, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    gid1,
    gid1->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    gid2,
    gid2->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *result = (memcmp(gid1->data, gid2->data, RMW_GID_STORAGE_SIZE) == 0);
  return RMW_RET_OK;
}

}  // extern "C"
