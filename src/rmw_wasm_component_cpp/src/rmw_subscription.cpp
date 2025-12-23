#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/dynamic_message_type_support.h"
#include "rmw/subscription_content_filter_options.h"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"
#include "rmw_wasm_component_cpp/rmw_subscription_data.hpp"
#include "rcutils/logging_macros.h"
#include <cstring>

extern "C" {

RMW_PUBLIC
rmw_subscription_t *
rmw_create_subscription(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_profile,
  const rmw_subscription_options_t * subscription_options)
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
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription_options, nullptr);

  // Create subscription data with message queue
  auto* sub_data = new rmw_wasm_component_cpp::SubscriptionData();
  sub_data->topic_name = topic_name;

  // Register in global registry for incoming message routing
  rmw_wasm_component_cpp::SubscriptionRegistry::instance().register_subscription(
    topic_name, sub_data);

  rmw_subscription_t * subscription = new rmw_subscription_t();
  subscription->implementation_identifier = rmw_wasm_component_cpp::identifier;
  subscription->data = sub_data;
  subscription->topic_name = topic_name;
  subscription->can_loan_messages = false;

  RCUTILS_LOG_DEBUG_NAMED("rmw_wasm", "Created subscription for topic: %s", topic_name);

  return subscription;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_subscription(rmw_node_t * node, rmw_subscription_t * subscription)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  if (subscription->data) {
    auto* sub_data = static_cast<rmw_wasm_component_cpp::SubscriptionData*>(subscription->data);
    // Unregister from global registry
    rmw_wasm_component_cpp::SubscriptionRegistry::instance().unregister_subscription(
      sub_data->topic_name, sub_data);
    delete sub_data;
  }
  delete subscription;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_count_matched_publishers(
  const rmw_subscription_t * subscription,
  size_t * publisher_count)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher_count, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *publisher_count = 0;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_get_actual_qos(
  const rmw_subscription_t * subscription,
  rmw_qos_profile_t * qos)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *qos = rmw_qos_profile_default;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take(
  const rmw_subscription_t * subscription,
  void * ros_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *taken = false;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_with_info(
  const rmw_subscription_t * subscription,
  void * ros_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(message_info, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *taken = false;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_serialized_message(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  auto* sub_data = static_cast<rmw_wasm_component_cpp::SubscriptionData*>(subscription->data);
  if (!sub_data) {
    *taken = false;
    return RMW_RET_OK;
  }

  // Try to get a message from the queue
  auto msg_data = sub_data->dequeue();
  if (msg_data.empty()) {
    *taken = false;
    return RMW_RET_OK;
  }

  // Copy data to serialized_message
  if (serialized_message->buffer_capacity < msg_data.size()) {
    rmw_ret_t ret = rmw_serialized_message_resize(serialized_message, msg_data.size());
    if (ret != RMW_RET_OK) {
      return ret;
    }
  }
  std::memcpy(serialized_message->buffer, msg_data.data(), msg_data.size());
  serialized_message->buffer_length = msg_data.size();

  *taken = true;
  RCUTILS_LOG_DEBUG_NAMED("rmw_wasm", "Took message from topic: %s", sub_data->topic_name.c_str());
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_serialized_message_with_info(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation)
{
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(message_info, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  // Use the same implementation as rmw_take_serialized_message
  rmw_ret_t ret = rmw_take_serialized_message(subscription, serialized_message, taken, allocation);
  if (ret != RMW_RET_OK) {
    return ret;
  }

  // Fill in message info with defaults
  if (*taken) {
    message_info->source_timestamp = 0;
    message_info->received_timestamp = 0;
    message_info->publication_sequence_number = 0;
    message_info->reception_sequence_number = 0;
    message_info->from_intra_process = false;
    std::memset(&message_info->publisher_gid, 0, sizeof(message_info->publisher_gid));
  }

  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_loaned_message(
  const rmw_subscription_t * subscription,
  void ** loaned_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation)
{
  (void)subscription;
  (void)loaned_message;
  (void)taken;
  (void)allocation;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_loaned_message_with_info(
  const rmw_subscription_t * subscription,
  void ** loaned_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation)
{
  (void)subscription;
  (void)loaned_message;
  (void)taken;
  (void)message_info;
  (void)allocation;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_return_loaned_message_from_subscription(
  const rmw_subscription_t * subscription,
  void * loaned_message)
{
  (void)subscription;
  (void)loaned_message;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_set_content_filter(
  rmw_subscription_t * subscription,
  const rmw_subscription_content_filter_options_t * options)
{
  (void)subscription;
  (void)options;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_get_content_filter(
  const rmw_subscription_t * subscription,
  rcutils_allocator_t * allocator,
  rmw_subscription_content_filter_options_t * options)
{
  (void)subscription;
  (void)allocator;
  (void)options;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_sequence(
  const rmw_subscription_t * subscription,
  size_t count,
  rmw_message_sequence_t * message_sequence,
  rmw_message_info_sequence_t * message_info_sequence,
  size_t * taken,
  rmw_subscription_allocation_t * allocation)
{
  (void)subscription;
  (void)count;
  (void)message_sequence;
  (void)message_info_sequence;
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  *taken = 0;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_dynamic_message_with_info(
  const rmw_subscription_t * subscription,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation)
{
  (void)subscription;
  (void)dynamic_message;
  (void)message_info;
  (void)allocation;
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  *taken = false;
  return RMW_RET_UNSUPPORTED;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_set_on_new_message_callback(
  rmw_subscription_t * subscription,
  rmw_event_callback_t callback,
  const void * user_data)
{
  (void)subscription;
  (void)callback;
  (void)user_data;
  return RMW_RET_OK;
}

}  // extern "C"
