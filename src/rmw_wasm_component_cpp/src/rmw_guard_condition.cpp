#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"
#include "rmw_wasm_component_cpp/rmw_guard.hpp"
#include "rcpputils/scope_exit.hpp"
#include "rcutils/allocator.h"

namespace rmw_wasm_component_cpp
{
    GuardCondition::GuardCondition() : triggered_(false) {}
    GuardCondition::~GuardCondition() {}
    bool GuardCondition::is_triggered() const { return triggered_.load(); }
    void GuardCondition::trigger() { triggered_.store(true); }
} // namespace rmw_wasm_component_cpp

extern "C" {

RMW_PUBLIC
rmw_guard_condition_t *
rmw_create_guard_condition(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return nullptr);
  
  rcutils_allocator_t * allocator = &context->options.allocator;
  RCUTILS_CHECK_ALLOCATOR(allocator, return nullptr);
  auto guard_condition =
    static_cast<rmw_guard_condition_t *>(allocator->zero_allocate(
      1, sizeof(rmw_guard_condition_t), allocator->state));
  RMW_CHECK_FOR_NULL_WITH_MSG(
    guard_condition,
    "unable to allocate memory for guard_condition",
    return nullptr);
  auto free_guard_condition = rcpputils::make_scope_exit(
    [guard_condition, allocator]() {
      allocator->deallocate(guard_condition, allocator->state);
    });

  guard_condition->implementation_identifier = rmw_wasm_component_cpp::identifier;
  guard_condition->context = context;

  guard_condition->data = allocator->zero_allocate(
    1, sizeof(rmw_wasm_component_cpp::GuardCondition), allocator->state);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    guard_condition->data,
    "unable to allocate memory for guard_condition data",
    return nullptr);
  auto free_guard_condition_data = rcpputils::make_scope_exit(
    [guard_condition, allocator]() {
      allocator->deallocate(guard_condition->data, allocator->state);
    });

  new (guard_condition->data) rmw_wasm_component_cpp::GuardCondition();
  auto destroy_guard_condition_data = rcpputils::make_scope_exit(
    [guard_condition, allocator]() {
      RMW_TRY_DESTRUCTOR_FROM_WITHIN_FAILURE(
        static_cast<rmw_wasm_component_cpp::GuardCondition *>(guard_condition->data)->~GuardCondition(),
        rmw_wasm_component_cpp::GuardCondition);
    });
  
  free_guard_condition.cancel();
  free_guard_condition_data.cancel();
  destroy_guard_condition_data.cancel();
  
  return guard_condition;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_guard_condition(rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition->context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition->data, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard_condition,
    guard_condition->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  const rcutils_allocator_t * allocator = &guard_condition->context->options.allocator;
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_ERROR);

  rmw_wasm_component_cpp::GuardCondition * guard_condition_data =
    static_cast<rmw_wasm_component_cpp::GuardCondition *>(guard_condition->data);
  if (nullptr == guard_condition_data) {
    RMW_SET_ERROR_MSG("Cannot cast guard condition data to rmw_wasm_component_cpp::GuardCondition");
    return RMW_RET_ERROR;
  }
  RMW_TRY_DESTRUCTOR_FROM_WITHIN_FAILURE(
    guard_condition_data->~GuardCondition(),
    rmw_wasm_component_cpp::GuardCondition);
  allocator->deallocate(guard_condition->data, allocator->state);
  allocator->deallocate(guard_condition, allocator->state);

  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_trigger_guard_condition(const rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition->context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition->data, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard_condition,
    guard_condition->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  rmw_wasm_component_cpp::GuardCondition * guard_condition_data =
    static_cast<rmw_wasm_component_cpp::GuardCondition *>(guard_condition->data);
  if (nullptr == guard_condition_data) {
    RMW_SET_ERROR_MSG("Cannot cast guard condition data to rmw_wasm_component_cpp::GuardCondition");
    return RMW_RET_ERROR;
  }
  guard_condition_data->trigger();
  return RMW_RET_OK;
}


}  // extern "C"
