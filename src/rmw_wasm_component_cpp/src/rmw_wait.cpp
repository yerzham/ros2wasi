#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"


extern const char * wasm_identifier;

extern "C" {

RMW_PUBLIC
rmw_wait_set_t *
rmw_create_wait_set(rmw_context_t * context, size_t max_conditions)
{
  (void)max_conditions;
  RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    wasm_identifier,
    return nullptr);
  
  rmw_wait_set_t * wait_set = new rmw_wait_set_t();
  wait_set->implementation_identifier = wasm_identifier;
  wait_set->data = nullptr;
  
  return wait_set;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_wait_set(rmw_wait_set_t * wait_set)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(wait_set, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    wait_set,
    wait_set->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  delete wait_set;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_wait(
  rmw_subscriptions_t * subscriptions,
  rmw_guard_conditions_t * guard_conditions,
  rmw_services_t * services,
  rmw_clients_t * clients,
  rmw_events_t * events,
  rmw_wait_set_t * wait_set,
  const rmw_time_t * wait_timeout)
{
  (void)wait_timeout;
  RMW_CHECK_ARGUMENT_FOR_NULL(wait_set, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    wait_set,
    wait_set->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  // Clear all wait sets
  if (subscriptions) {
    for (size_t i = 0; i < subscriptions->subscriber_count; ++i) {
      subscriptions->subscribers[i] = nullptr;
    }
  }
  if (guard_conditions) {
    for (size_t i = 0; i < guard_conditions->guard_condition_count; ++i) {
      guard_conditions->guard_conditions[i] = nullptr;
    }
  }
  if (services) {
    for (size_t i = 0; i < services->service_count; ++i) {
      services->services[i] = nullptr;
    }
  }
  if (clients) {
    for (size_t i = 0; i < clients->client_count; ++i) {
      clients->clients[i] = nullptr;
    }
  }
  if (events) {
    for (size_t i = 0; i < events->event_count; ++i) {
      events->events[i] = nullptr;
    }
  }
  
  return RMW_RET_OK;
}


}  // extern "C"
