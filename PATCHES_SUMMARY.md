# ROS2 WASI Patches Summary

This document explains why each patch in the `patches/` directory exists for WebAssembly (WASI) support.

## `ament_ament_index.patch`
**Why**: Forces static library builds for WASI instead of shared libraries.

**Reason**: WASI/WebAssembly has limited support for dynamic shared libraries. Building as static libraries avoids RTTI (Run-Time Type Information) and typeinfo issues that occur when using shared libraries in WebAssembly environments.

## `ros2_rcpputils.patch`
**Why**: Excludes process management code and temporary directory creation functions.

**Reason**:
- WASI doesn't support process creation or management APIs
- Functions like `mkdtemp()` for creating temporary directories are not available in WASI's restricted filesystem model
- The `process.cpp` file is entirely excluded from WASI builds

## `ros2_rcutils.patch`
**Why**: Stubs out process-related functions and provides WASI-specific implementations.

**Reason**:
- `rcutils_get_executable_name()` returns a hardcoded `"wasm_module"` since WASI doesn't have `program_invocation_name` or equivalent
- `rcutils_start_process()` and `rcutils_process_wait()` return errors because WASI completely lacks process creation/spawning capabilities
- WASI runs in a sandboxed environment with no access to host process APIs

## `ros2_libyaml_vendor.patch`
**Why**: Forces static library build and passes WASI toolchain configuration.

**Reason**:
- Ensures libyaml is built with the wasi-sdk toolchain by passing `CMAKE_TOOLCHAIN_FILE`
- Builds as static library to avoid WebAssembly dynamic linking issues
- Adds `-fPIC` (Position Independent Code) for proper linking

## `ros-tooling_libstatistics_collector.patch`
**Why**: Forces static library builds for WASI.

**Reason**: Similar to ament_ament_index - avoids vtable and RTTI issues that occur with C++ virtual functions and dynamic libraries in WebAssembly.

## `ros2_spdlog_vendor.patch`
**Why**: Links WASI-specific emulated libraries required by spdlog.

**Reason**: WASI lacks many POSIX APIs that spdlog depends on. The wasi-sdk provides emulated versions:
- `libwasi-emulated-mman` - Memory mapping functions (`mmap`, `munmap`)
- `libwasi-emulated-getpid` - Process ID functions (`getpid`)
- `libwasi-emulated-process-clocks` - Process timing/clock functions
- `libunwind` + `libc++abi` - C++ exception handling and stack unwinding (required for C++ exceptions in WebAssembly)

These emulations are necessary because WASI is a capability-based security model that doesn't expose these system-level APIs directly.

## `ros2_rcl_logging.patch`
**Why**: Exports spdlog dependencies for downstream packages.

**Reason**: Downstream packages need to locate and link against spdlog and spdlog_vendor. Without explicit export, CMake can't find these dependencies in the WASI build environment.

## `ros2_rcl.patch`
**Why**: Exports rcl_logging_spdlog and tracetools as dependencies.

**Reason**: These libraries were linked as `PRIVATE` dependencies but are actually needed by downstream packages. The WASI build requires explicit dependency exports to properly propagate linking requirements through the dependency chain.

## `ros2_rclcpp.patch`
**Why**: Exports ament_index_cpp, disables thread creation, and replaces POSIX semaphores with condition variables for WASI.

**Reason**:
- **Dependency export**: ament_index_cpp was linked privately but is needed by downstream packages
- **Threading**: WASI-P2 does NOT support creating new threads via `std::thread`. The patch wraps all `std::thread` creation and `join()` calls with `#ifndef __wasi__` preprocessor guards in:
  - `graph_listener.cpp` - Graph change listener thread
  - `signal_handler.cpp` - Signal handler deferred execution thread
  - `time_source.cpp` - Clock executor thread
  - `timers_manager.cpp` - Timers management thread
  - `multi_threaded_executor.cpp` - Multi-threaded executor worker threads (falls back to single-threaded in WASM)
  - `component_manager_isolated.hpp` - Component manager dedicated executor threads
- **Semaphores**: WASI doesn't support POSIX semaphores (`sem_t`, `sem_init`, `sem_wait`, `sem_post`). The patch replaces them with C++ standard library condition variables (`std::condition_variable`) + mutex in `signal_handler.cpp`:
  - `setup_wait_for_signal()` - Initializes condition variable state
  - `wait_for_signal()` - Uses `cv.wait()` instead of `sem_wait()`
  - `notify_signal_handler()` - Uses `cv.notify_one()` instead of `sem_post()`
  - `teardown_wait_for_signal()` - Automatic cleanup (no manual destruction needed)
- Mutexes and condition variables work correctly in WASI's single-threaded environment

---

## `ros2_rcl_logging.patch`
**Why**: Disables spdlog's periodic flusher thread and exports dependencies for WASI.

**Status**: PATCHED

**Error Message**:
```
[ERROR] Caught std::exception: thread constructor failed: Resource temporarily unavailable
[ERROR] [rclcpp]: unhandled exception in ~Context(): recursive_mutex lock failed: Resource deadlock would occur
```

**Root Cause**: The `spdlog` logging library creates a background thread for periodic log flushing.

**Call Chain**:
```
rclcpp::init()
  → Context::init()                                    [context.cpp:238]
    → rcl_logging_configure_with_output_handler()      [context.cpp:266]
      → rcl_logging_external_initialize()              [rcl_logging_spdlog.cpp:98]
        → spdlog::flush_every(std::chrono::seconds(5)) [rcl_logging_spdlog.cpp:202]
          → registry::flush_every()                    [registry.h:65]
            → periodic_worker constructor              [registry.h:69]
              → std::thread(...)                       [periodic_worker.h:32] ← THREAD CREATED HERE
```

**Files Involved**:
- `/ros2_ws/src/ros2/rcl_logging/rcl_logging_spdlog/src/rcl_logging_spdlog.cpp:202` - Calls `spdlog::flush_every()`
- `spdlog/details/registry.h:69` - Creates `periodic_worker`
- `spdlog/details/periodic_worker.h:32` - Spawns the actual `std::thread`

**Why rclcpp patches don't fix this**: The thread is created inside the **spdlog library**, not in rclcpp code. The `#ifndef __wasi__` guards only protect rclcpp's own thread creations, not third-party libraries.

**Solution Applied**:
Wrapped the `spdlog::flush_every()` call with `#ifndef __wasi__` in `rcl_logging_spdlog.cpp:202`

**Alternative Workarounds** (if patch not applied):
1. **Disable logging**: Pass `auto_initialize_logging=false` in `InitOptions`
2. **Set environment variable**: `RCL_LOGGING_SPDLOG_EXPERIMENTAL_OLD_FLUSHING_BEHAVIOR=1`

**Secondary Error Explanation**:
The `recursive_mutex lock failed` error occurs because:
1. Thread creation fails with an exception
2. Exception triggers Context cleanup/destruction
3. `~Context()` tries to lock `init_mutex_` (a recursive_mutex)
4. The mutex implementation in WASM fails during this exceptional state

---

## General WASI/wasi-sdk Limitations

The patches collectively work around these fundamental WASI restrictions:

1. **No dynamic linking**: WebAssembly has limited shared library support → use static libraries
2. **No process APIs**: No `fork()`, `exec()`, `wait()`, process creation
3. **No thread creation**: WASI-P2 doesn't support `std::thread` creation, though mutexes and condition variables work in single-threaded mode
4. **No POSIX semaphores**: WASI lacks `sem_t`, `sem_init`, `sem_wait`, `sem_post` → use C++ standard library condition variables instead
5. **Sandboxed environment**: No direct access to host system resources
6. **Emulation required**: Many standard library features need explicit emulation libraries from wasi-sdk
