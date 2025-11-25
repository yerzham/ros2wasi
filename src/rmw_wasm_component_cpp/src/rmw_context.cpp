#include "rmw_wasm_component_cpp/rmw_context.hpp"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"
#include "ros2_wit/rmw_cpp.h"
#include "rcutils/logging_macros.h"

rmw_context_impl_s::rmw_context_impl_s() : client_(wit::ResourceImportBase()) , graph_guard_condition_data_() {
    auto result = wasi::messaging::types::Client::Connect("ros2");
    if (!result) {
        RCUTILS_LOG_ERROR("Failed to connect to messaging client");
        return;
    }
    client_ = std::move(result.value());
    graph_guard_condition_ = std::make_unique<rmw_guard_condition_t>();
    graph_guard_condition_->implementation_identifier = rmw_wasm_component_cpp::identifier;
    graph_guard_condition_->data = &graph_guard_condition_data_;
}
rmw_context_impl_s::~rmw_context_impl_s() {
    auto result = client_.Disconnect();
    if (!result) {
        RCUTILS_LOG_ERROR("Failed to disconnect from messaging client");
    }
}
void rmw_context_impl_s::shutdown() {
    is_shutdown_ = true;
}
bool rmw_context_impl_s::is_shutdown() {
    return is_shutdown_;
}
bool rmw_context_impl_s::create_node(const rmw_node_t *node) {
    if (is_shutdown_) {
        RCUTILS_LOG_ERROR("Context is shutdown");
        return false;
    }
    if (nodes_.count(node) > 0) {
        // Node already exists.
        return false;
    }
    auto result = std::make_unique<rmw_node_data_t>();
    auto [_, success] = nodes_.insert({node, std::move(result)});
    if (!success) {
        RCUTILS_LOG_ERROR("Failed to insert node into map");
        return false;
    }
    return true;
}
std::unique_ptr<rmw_node_data_t> rmw_context_impl_s::get_node_data(const rmw_node_t * const node)
{
    auto node_it = nodes_.find(node);
    if (node_it == nodes_.end()) {
        return nullptr;
    }
    return std::move(node_it->second);
}
bool rmw_context_impl_s::delete_node_data(const rmw_node_t * const node)
{
    auto result = nodes_.erase(node);
    if (result == 0) {
        RCUTILS_LOG_WARN("Node not found");
        return false;
    }
    return true;
}
const rmw_guard_condition_t * rmw_context_impl_s::get_graph_guard_condition() {
    return graph_guard_condition_.get();
}