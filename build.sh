#!/bin/bash

debug_mode=0
build_type=Release
RMW_IMPLEMENTATION=rmw_wasm_component_cpp
CMAKE_C_FLAGS="-fwasm-exceptions -mllvm -wasm-use-legacy-eh=false -D_WASI_EMULATED_MMAN -D_WASI_EMULATED_GETPID -D_WASI_EMULATED_PROCESS_CLOCKS -D_WASI_EMULATED_SIGNAL"
CMAKE_CXX_FLAGS="-fwasm-exceptions -mllvm -wasm-use-legacy-eh=false -D_WASI_EMULATED_MMAN -D_WASI_EMULATED_GETPID -D_WASI_EMULATED_PROCESS_CLOCKS -D_WASI_EMULATED_SIGNAL -Wno-c2y-extensions"
WASI_SDK=/opt/wasi-sdk
WASI_SYSROOT="${WASI_SDK}/share/wasi-sysroot/lib/wasm32-wasip2"
CMAKE_EXE_LINKER_FLAGS="-lc++abi -lunwind -lwasi-emulated-mman -lwasi-emulated-getpid -lwasi-emulated-process-clocks -lwasi-emulated-signal"
CMAKE_SHARED_LINKER_FLAGS="${WASI_SYSROOT}/libc++abi.a ${WASI_SYSROOT}/libunwind.a ${WASI_SYSROOT}/libwasi-emulated-mman.so ${WASI_SYSROOT}/libwasi-emulated-getpid.so ${WASI_SYSROOT}/libwasi-emulated-process-clocks.so ${WASI_SYSROOT}/libwasi-emulated-signal.so"

# Clear host ROS environment variables to prevent finding host packages
unset ROS_DISTRO
unset ROS_VERSION
unset ROS_PYTHON_VERSION
unset AMENT_PREFIX_PATH
unset CMAKE_PREFIX_PATH
# Set only /ros2_ws/install - completely exclude /opt/ros/kilted from CMake
export AMENT_PREFIX_PATH=/ros2_ws/install
export CMAKE_PREFIX_PATH=/ros2_ws/install
# But add host ROS Python tools to PATH so they can be executed
export PATH=/opt/ros/kilted/bin:/opt/ros/kilted/lib/rosidl_generator_type_description:$PATH
export PYTHONPATH=/opt/ros/kilted/lib/python3.12/site-packages:$PYTHONPATH

# Create dummy rosidl_generator_py to prevent Python binding generation
mkdir -p install/share/rosidl_generator_py/cmake
cat > install/share/rosidl_generator_py/cmake/rosidl_generator_py_generate_interfaces.cmake << 'EOF'
# Dummy file to prevent host ROS rosidl_generator_py from being used
# Python bindings are not needed for WASM build
function(rosidl_generator_py_generate_interfaces)
  # Do nothing - skip Python generation
endfunction()
EOF

cat > install/share/rosidl_generator_py/cmake/rosidl_generator_pyConfig.cmake << 'EOF'
# Dummy config file to prevent host ROS rosidl_generator_py from being used
set(rosidl_generator_py_FOUND TRUE)
include("${CMAKE_CURRENT_LIST_DIR}/rosidl_generator_py_generate_interfaces.cmake")
EOF

colcon build \
    --packages-skip-build-finished \
    --cmake-args \
        -DCMAKE_TOOLCHAIN_FILE="${WASI_SDK}/share/cmake/wasi-sdk-p2.cmake" \
        -DBUILD_TESTING=OFF \
        -DBUILD_SHARED_LIBS=ON \
        -DCMAKE_VERBOSE_MAKEFILE=${debug_mode} \
        -DRMW_IMPLEMENTATION=${RMW_IMPLEMENTATION} \
        -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ON \
        -DCMAKE_CROSSCOMPILING=TRUE \
        -DCMAKE_FIND_DEBUG_MODE=${debug_mode} \
        -DFORCE_BUILD_VENDOR_PKG=ON \
        -DCMAKE_BUILD_TYPE=${build_type} \
        -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}" \
        -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}" \
        -DCMAKE_EXE_LINKER_FLAGS="${CMAKE_EXE_LINKER_FLAGS}" \
        -DCMAKE_SHARED_LINKER_FLAGS="${CMAKE_SHARED_LINKER_FLAGS}" \
        -DBENCHMARK_CXX_LINKER_FLAGS="${CMAKE_EXE_LINKER_FLAGS}" \
        -DTRACETOOLS_DISABLED=TRUE \
        -DTRACETOOLS_STATUS_CHECKING_TOOL=OFF \
        -Dyaml_FOUND=FALSE \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -Wno-dev \
    --packages-up-to rclcpp
