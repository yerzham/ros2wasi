#ifndef RMW_WASM_COMPONENT_CPP_RMW_GUARD_HPP
#define RMW_WASM_COMPONENT_CPP_RMW_GUARD_HPP

#include <atomic>
namespace rmw_wasm_component_cpp
{

class GuardCondition
{
public:
    GuardCondition();
    ~GuardCondition();
    bool is_triggered() const;
    void trigger();
private:
    std::atomic_bool triggered_{ false };
};

} // namespace rmw_wasm_component_cpp

#endif // RMW_WASM_COMPONENT_CPP_RMW_GUARD_HPP