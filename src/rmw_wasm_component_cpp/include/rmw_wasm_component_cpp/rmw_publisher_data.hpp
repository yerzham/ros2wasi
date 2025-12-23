#ifndef RMW_WASM_COMPONENT_CPP_RMW_PUBLISHER_DATA_HPP
#define RMW_WASM_COMPONENT_CPP_RMW_PUBLISHER_DATA_HPP

#include <string>
#include "rosidl_typesupport_interface/macros.h"

// Forward declaration
struct rmw_context_impl_s;
struct rosidl_message_type_support_t;

namespace rmw_wasm_component_cpp {

/// Data stored in publisher->data
struct PublisherData {
    std::string topic_name;
    rmw_context_impl_s* context;
    const rosidl_message_type_support_t* type_support;
};

}  // namespace rmw_wasm_component_cpp

#endif  // RMW_WASM_COMPONENT_CPP_RMW_PUBLISHER_DATA_HPP
