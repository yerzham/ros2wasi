#ifndef RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP
#define RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP

#include <unordered_map>
#include "rmw/rmw.h"
#include "ros2_wit/rmw_cpp.h"
#include "rmw_wasm_component_cpp/rmw_guard.hpp"

typedef struct rmw_node_data_s 
{
} rmw_node_data_t;

struct rmw_context_impl_s final
{
public:
    rmw_context_impl_s();
    ~rmw_context_impl_s();
    void shutdown();
    bool is_shutdown();
    bool create_node(const rmw_node_t *node);
    std::unique_ptr<rmw_node_data_t> get_node_data(const rmw_node_t * const node);
    bool delete_node_data(const rmw_node_t * const node);
    const rmw_guard_condition_t * get_graph_guard_condition();
private:
    bool is_shutdown_{ false };
    std::unordered_map<const rmw_node_t *, std::unique_ptr<rmw_node_data_t>> nodes_;
    wasi::messaging::types::Client client_;
    std::unique_ptr<rmw_guard_condition_t> graph_guard_condition_;
    rmw_wasm_component_cpp::GuardCondition graph_guard_condition_data_;
};
#endif // RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP