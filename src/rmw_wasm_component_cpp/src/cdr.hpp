// Copyright 2024 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMW_WASM_COMPONENT_CPP__CDR_HPP_
#define RMW_WASM_COMPONENT_CPP__CDR_HPP_

#include <memory>

#include "fastcdr/Cdr.h"
#include "fastcdr/FastBuffer.h"

namespace rmw_wasm_component_cpp
{
///=============================================================================
// Wrapper class around eprosima::fastcdr::Cdr to provide proper initialization
class Cdr final
{
public:
  explicit Cdr(eprosima::fastcdr::FastBuffer & buffer)
  : cdr_(buffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN, eprosima::fastcdr::CdrVersion::XCDRv1)
  {
  }

  eprosima::fastcdr::Cdr & get_cdr()
  {
    return cdr_;
  }

private:
  eprosima::fastcdr::Cdr cdr_;
};

}  // namespace rmw_wasm_component_cpp

#endif  // RMW_WASM_COMPONENT_CPP__CDR_HPP_
