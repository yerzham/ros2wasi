#include "rmw/rmw.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/qos_profiles.h"
#include "rmw_wasm_component_cpp/rmw_indentifier.hpp"

extern "C" {

RMW_PUBLIC
rmw_ret_t
rmw_qos_profile_check_compatible(
  const rmw_qos_profile_t publisher_profile,
  const rmw_qos_profile_t subscription_profile,
  rmw_qos_compatibility_type_t * compatibility,
  char * reason,
  size_t reason_size)
{
  (void)publisher_profile;
  (void)subscription_profile;
  (void)reason;
  (void)reason_size;
  RMW_CHECK_ARGUMENT_FOR_NULL(compatibility, RMW_RET_INVALID_ARGUMENT);
  
  *compatibility = RMW_QOS_COMPATIBILITY_OK;
  return RMW_RET_OK;
}


}  // extern "C"
