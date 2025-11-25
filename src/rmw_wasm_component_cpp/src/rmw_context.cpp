#include "rmw_wasm_component_cpp/rmw_context.hpp"



rmw_context_impl_s::rmw_context_impl_s() {}
rmw_context_impl_s::~rmw_context_impl_s() {}
void rmw_context_impl_s::shutdown() {
    is_shutdown_ = true;
}
bool rmw_context_impl_s::is_shutdown() {
    return is_shutdown_;
}