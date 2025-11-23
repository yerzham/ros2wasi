#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"


extern const char * wasm_identifier;

extern "C" {

RMW_PUBLIC
rmw_guard_condition_t *
rmw_create_guard_condition(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    wasm_identifier,
    return nullptr);
  
  rmw_guard_condition_t * guard_condition = new rmw_guard_condition_t();
  guard_condition->implementation_identifier = wasm_identifier;
  guard_condition->data = nullptr;
  guard_condition->context = context;
  
  return guard_condition;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_guard_condition(rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard_condition,
    guard_condition->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  delete guard_condition;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_trigger_guard_condition(const rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard_condition,
    guard_condition->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}


}  // extern "C"
