#include "rcutils/logging_macros.h"
#include "ros2_wit/rmw_cpp.h"

namespace exports::wasi::messaging::incoming_handler
{

std::expected<void, ::wasi::messaging::types::Error>
Handle(::wasi::messaging::types::Message && message)
{
  (void)message;
  RCUTILS_LOG_DEBUG_NAMED(
    "rmw_wasm_component_cpp",
    "incoming_handler::Handle invoked but message processing is not implemented yet");

  return std::expected<void, ::wasi::messaging::types::Error>();
}

}  // namespace exports::wasi::messaging::incoming_handler

