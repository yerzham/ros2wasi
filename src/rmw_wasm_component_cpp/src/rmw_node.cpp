#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/validate_node_name.h"
#include "rmw/validate_namespace.h"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"

extern "C" {

RMW_PUBLIC
rmw_node_t *
rmw_create_node(
  rmw_context_t * context,
  const char * name,
  const char * namespace_)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(name, nullptr);
  RMW_CHECK_ARGUMENT_FOR_NULL(namespace_, nullptr);
  
  int validation_result = RMW_NODE_NAME_VALID;
  rmw_ret_t ret = rmw_validate_node_name(name, &validation_result, nullptr);
  if (RMW_RET_OK != ret) {
    return nullptr;
  }
  if (RMW_NODE_NAME_VALID != validation_result) {
    RMW_SET_ERROR_MSG("invalid node name");
    return nullptr;
  }
  
  validation_result = RMW_NAMESPACE_VALID;
  ret = rmw_validate_namespace(namespace_, &validation_result, nullptr);
  if (RMW_RET_OK != ret) {
    return nullptr;
  }
  if (RMW_NAMESPACE_VALID != validation_result) {
    RMW_SET_ERROR_MSG("invalid node namespace");
    return nullptr;
  }
  
  rmw_node_t * node = new rmw_node_t();
  node->implementation_identifier = rmw_wasm_component_cpp::identifier;
  node->data = nullptr;
  node->name = name;
  node->namespace_ = namespace_;
  node->context = context;
  
  return node;
}

RMW_PUBLIC
rmw_ret_t
rmw_destroy_node(rmw_node_t * node)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  
  delete node;
  return RMW_RET_OK;
}

const rmw_guard_condition_t *
rmw_node_get_graph_guard_condition(const rmw_node_t * node)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    rmw_wasm_component_cpp::identifier,
    return nullptr);
  
  return nullptr;
}


}  // extern "C"
