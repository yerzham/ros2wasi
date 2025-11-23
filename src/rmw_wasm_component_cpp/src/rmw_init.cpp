#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/init.h"
#include "rmw/init_options.h"
#include "rmw/features.h"
#include "rmw/dynamic_message_type_support.h"
#include "rcutils/logging_macros.h"

const char * wasm_identifier = "rmw_wasm_component_cpp";

extern "C" {

RMW_PUBLIC
const char *
RMW_PUBLIC
rmw_get_implementation_identifier(void)
{
  return wasm_identifier;
}

RMW_PUBLIC
const char *
RMW_PUBLIC
rmw_get_serialization_format(void)
{
  return wasm_identifier;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_init_options_init(rmw_init_options_t * init_options, rcutils_allocator_t allocator)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(init_options, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(&allocator, return RMW_RET_INVALID_ARGUMENT);
  if (init_options->implementation_identifier) {
    RMW_SET_ERROR_MSG("init_options already initialized");
    return RMW_RET_INVALID_ARGUMENT;
  }
  init_options->instance_id = 0;
  init_options->implementation_identifier = wasm_identifier;
  init_options->allocator = allocator;
  init_options->impl = nullptr;
  init_options->enclave = nullptr;
  init_options->domain_id = RMW_DEFAULT_DOMAIN_ID;
  init_options->security_options = rmw_get_zero_initialized_security_options();
  init_options->discovery_options = rmw_get_zero_initialized_discovery_options();
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_init_options_copy(const rmw_init_options_t * src, rmw_init_options_t * dst)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    src,
    src->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  if (dst->implementation_identifier) {
    RMW_SET_ERROR_MSG("dst already initialized");
    return RMW_RET_INVALID_ARGUMENT;
  }
  *dst = *src;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_init_options_fini(rmw_init_options_t * init_options)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(init_options, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    init_options,
    init_options->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  *init_options = rmw_get_zero_initialized_init_options();
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_init(const rmw_init_options_t * options, rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(options, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    options,
    options->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  context->instance_id = options->instance_id;
  context->implementation_identifier = wasm_identifier;
  context->impl = nullptr;
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_shutdown(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_context_fini(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    wasm_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  *context = rmw_get_zero_initialized_context();
  return RMW_RET_OK;
}

RMW_PUBLIC
bool
RMW_PUBLIC
rmw_feature_supported(rmw_feature_t feature)
{
  // Most features are not supported in the stub WASM RMW
  (void)feature;
  return false;
}

RMW_PUBLIC
rmw_ret_t
RMW_PUBLIC
rmw_serialization_support_init(
  const char * serialization_lib_name,
  rcutils_allocator_t * allocator,
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support)
{
  (void)serialization_lib_name;
  (void)allocator;
  (void)serialization_support;
  return RMW_RET_UNSUPPORTED;
}

}  // extern "C"
