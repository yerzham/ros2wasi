#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/event.h"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"


extern "C" {

RMW_PUBLIC
rmw_ret_t
rmw_publisher_event_init(
  rmw_event_t * rmw_event,
  const rmw_publisher_t * publisher,
  rmw_event_type_t event_type)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(rmw_event, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    publisher,
    publisher->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  rmw_event->implementation_identifier = rmw_wasm_component_cpp::identifier;
  rmw_event->data = nullptr;
  rmw_event->event_type = event_type;
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_subscription_event_init(
  rmw_event_t * rmw_event,
  const rmw_subscription_t * subscription,
  rmw_event_type_t event_type)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(rmw_event, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    subscription,
    subscription->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  rmw_event->implementation_identifier = rmw_wasm_component_cpp::identifier;
  rmw_event->data = nullptr;
  rmw_event->event_type = event_type;
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_take_event(
  const rmw_event_t * event_handle,
  void * event_info,
  bool * taken)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(event_handle, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(event_info, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    event_handle,
    event_handle->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *taken = false;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_event_set_callback(
  rmw_event_t * event,
  rmw_event_callback_t callback,
  const void * user_data)
{
  (void)callback;
  (void)user_data;
  RMW_CHECK_ARGUMENT_FOR_NULL(event, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    event,
    event->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}


}  // extern "C"
