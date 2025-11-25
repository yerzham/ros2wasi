#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/init.h"
#include "rmw/init_options.h"
#include "rmw/features.h"
#include "rmw/dynamic_message_type_support.h"
#include "rcpputils/scope_exit.hpp"
#include "rcutils/logging_macros.h"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"
#include "rmw_wasm_component_cpp/rmw_context.hpp"
#include "rcutils/allocator.h"

extern "C" {

RMW_PUBLIC
const char *
rmw_get_implementation_identifier(void)
{
  return rmw_wasm_component_cpp::identifier;
}

RMW_PUBLIC
const char *
rmw_get_serialization_format(void)
{
  return rmw_wasm_component_cpp::identifier;
}

RMW_PUBLIC
rmw_ret_t
rmw_init_options_init(rmw_init_options_t * init_options, rcutils_allocator_t allocator)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(init_options, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(&allocator, return RMW_RET_INVALID_ARGUMENT);
  if (nullptr != init_options->implementation_identifier) {
    RMW_SET_ERROR_MSG("init_options already initialized");
    return RMW_RET_INVALID_ARGUMENT;
  }

  memset(init_options, 0, sizeof(rmw_init_options_t));
  init_options->instance_id = 0;
  init_options->implementation_identifier = rmw_wasm_component_cpp::identifier;
  init_options->allocator = allocator;
  init_options->impl = nullptr;
  init_options->enclave = nullptr;
  init_options->domain_id = RMW_DEFAULT_DOMAIN_ID;
  init_options->discovery_options = rmw_get_zero_initialized_discovery_options();
  init_options->security_options = rmw_get_default_security_options();
  return rmw_discovery_options_init(&(init_options->discovery_options), 0, &allocator);
}

RMW_PUBLIC
rmw_ret_t
rmw_init_options_copy(const rmw_init_options_t * src, rmw_init_options_t * dst)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    src->implementation_identifier,
    "expected initialized src",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    src,
    src->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    dst->implementation_identifier,
    "expected zero-initialized dst",
    return RMW_RET_INVALID_ARGUMENT);
  rcutils_allocator_t allocator = src->allocator;
  RCUTILS_CHECK_ALLOCATOR(&allocator, return RMW_RET_INVALID_ARGUMENT);

  rmw_init_options_t tmp;
  rmw_ret_t ret;
  memcpy(&tmp, src, sizeof(rmw_init_options_t));
  if (nullptr != src->enclave) {
    ret = rmw_enclave_options_copy(src->enclave, &allocator, &tmp.enclave);
    if (RMW_RET_OK != ret) {
      return ret;
    }
  }

  tmp.security_options = rmw_get_zero_initialized_security_options();
  ret =
    rmw_security_options_copy(&src->security_options, &allocator, &tmp.security_options);
  if (RMW_RET_OK != ret) {
    rmw_enclave_options_fini(tmp.enclave, &allocator);
    // Error already set
    return ret;
  }

  tmp.discovery_options = rmw_get_zero_initialized_discovery_options();
  ret = rmw_discovery_options_copy(&src->discovery_options, &allocator, &tmp.discovery_options);
  if (RMW_RET_OK != ret) {
    rmw_enclave_options_fini(tmp.enclave, &allocator);
    rmw_security_options_fini(&tmp.security_options, &allocator);
    return ret;
  }

  tmp.allocator = allocator;
  tmp.impl = nullptr;

  *dst = tmp;
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_init_options_fini(rmw_init_options_t * init_options)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(init_options, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    init_options->implementation_identifier,
    "expected initialized init options",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    init_options,
    init_options->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
    rcutils_allocator_t allocator = init_options->allocator;
    RCUTILS_CHECK_ALLOCATOR(&allocator, return RMW_RET_INVALID_ARGUMENT);
  
    rmw_ret_t ret;
    if (init_options->enclave != NULL) {
      ret = rmw_enclave_options_fini(init_options->enclave, &allocator);
      if (ret != RMW_RET_OK) {
        return ret;
      }
    }
    ret = rmw_security_options_fini(&init_options->security_options, &allocator);
    if (ret != RMW_RET_OK) {
      return ret;
    }
    ret = rmw_discovery_options_fini(&init_options->discovery_options);
    if (ret != RMW_RET_OK) {
      return ret;
    }
    *init_options = rmw_get_zero_initialized_init_options();
    return ret;
}

RMW_PUBLIC
rmw_ret_t
rmw_init(const rmw_init_options_t * options, rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(options, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    options->implementation_identifier,
    "expected initialized options",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    context->implementation_identifier,
    "expected initialized context",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    options,
    options->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    options->enclave,
    "expected non-null enclave",
    return RMW_RET_INVALID_ARGUMENT);

  if (options->domain_id >= UINT32_MAX && options->domain_id != RMW_DEFAULT_DOMAIN_ID) {
    RMW_SET_ERROR_MSG("domain id out of range");
    return RMW_RET_INVALID_ARGUMENT;
  }

  const rcutils_allocator_t allocator = options->allocator;
  auto restore_context = rcpputils::make_scope_exit(
    [context]() {*context = rmw_get_zero_initialized_context();});

  context->instance_id = options->instance_id;
  context->implementation_identifier = rmw_wasm_component_cpp::identifier;
  // No custom handling of RMW_DEFAULT_DOMAIN_ID. Simply use a reasonable domain id.
  context->actual_domain_id =
    RMW_DEFAULT_DOMAIN_ID != options->domain_id ? options->domain_id : 0u;

  context->impl = new (std::nothrow) rmw_context_impl_t();
  if (nullptr == context->impl) {
    RMW_SET_ERROR_MSG("failed to allocate context impl");
    return RMW_RET_BAD_ALLOC;
  }
  auto free_impl = rcpputils::make_scope_exit(
    [context]() {delete context->impl; context->impl = nullptr;});

  rmw_ret_t ret;
  if ((ret = rmw_init_options_copy(options, &context->options)) != RMW_RET_OK) {
    return ret;
  }
  auto free_options = rcpputils::make_scope_exit(
    [context]() {
      rmw_ret_t ret = rmw_init_options_fini(&context->options);
      if (ret != RMW_RET_OK) {
        RMW_SAFE_FWRITE_TO_STDERR("Failed to cleanup context options during error handling");
      }
    });

  free_impl.cancel();
  free_options.cancel();
  restore_context.cancel();
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_shutdown(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    context->impl,
    "expected initialized context",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  context->impl->shutdown();
  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_context_fini(rmw_context_t * context)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    context->impl,
    "expected initialized context",
    return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  if (!context->impl->is_shutdown()) {
    RCUTILS_SET_ERROR_MSG("context has not been shutdown");
    return RMW_RET_INVALID_ARGUMENT;
  }
  rmw_ret_t ret = rmw_init_options_fini(&context->options);
  delete context->impl;
  *context = rmw_get_zero_initialized_context();

  return ret;
}

RMW_PUBLIC
bool
rmw_feature_supported(rmw_feature_t feature)
{
  // Most features are not supported in the stub WASM RMW
  (void)feature;
  return false;
}

RMW_PUBLIC
rmw_ret_t
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
