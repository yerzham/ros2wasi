#include "rcutils/logging_macros.h"
#include "ros2_wit/rmw_cpp.h"
#include "rmw_wasm_component_cpp/rmw_subscription_data.hpp"
#include <string>

namespace exports::wasi::messaging::incoming_handler
{

std::expected<void, ::wasi::messaging::types::Error>
Handle(::wasi::messaging::types::Message && message)
{
  // Get topic from message
  auto topic_opt = message.Topic();
  if (!topic_opt) {
    RCUTILS_LOG_WARN_NAMED("rmw_wasm", "Received message without topic, ignoring");
    return std::expected<void, ::wasi::messaging::types::Error>();
  }

  std::string topic(topic_opt->data(), topic_opt->size());

  // Get message data
  auto data = message.Data();
  std::vector<uint8_t> msg_data(data.data(), data.data() + data.size());

  // Route to all subscriptions for this topic
  rmw_wasm_component_cpp::SubscriptionRegistry::instance().route_message(topic, msg_data);

  RCUTILS_LOG_DEBUG_NAMED("rmw_wasm", "Routed message to topic: %s (size: %zu)",
    topic.c_str(), msg_data.size());

  return std::expected<void, ::wasi::messaging::types::Error>();
}

}  // namespace exports::wasi::messaging::incoming_handler

