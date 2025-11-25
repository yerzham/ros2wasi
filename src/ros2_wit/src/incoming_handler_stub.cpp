#include "ros2_wit/rmw_cpp.h"

namespace exports::wasi::messaging::incoming_handler
{

__attribute__((weak))
std::expected<void, ::wasi::messaging::types::Error>
Handle(::wasi::messaging::types::Message && message)
{
  (void)message;
  return std::expected<void, ::wasi::messaging::types::Error>();
}

}  // namespace exports::wasi::messaging::incoming_handler

