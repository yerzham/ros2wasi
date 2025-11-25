#ifndef RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP
#define RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP

struct rmw_context_impl_s final
{
public:
    rmw_context_impl_s();
    ~rmw_context_impl_s();
    void shutdown();
    bool is_shutdown();
private:
    bool is_shutdown_{ false };
};

#endif // RMW_WASM_COMPONENT_CPP_RMW_CONTEXT_HPP