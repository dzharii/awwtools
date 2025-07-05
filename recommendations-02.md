# How to improve compile time

2025-05-13

Prompt:

> I want you to analyze the entire codebase and find the way to optimize build time. look into Cmake files, suggest what I can do to make the build more optimal, more faster. use modern best practices and at the end only edit recommendations-02.md and put all recommendations there. start from the most impactful and add all.

# LLM Recommendations on how to improve compile and build time:

Below are ordered recommendations, from most impactful to fine-tuning tweaks:

1. **Enable Compiler-Level Caching**

   - Use `ccache` (GCC/Clang) or `sccache` (MSVC/Clang) by adding:
     ```cmake
     set(CMAKE_C_COMPILER_LAUNCHER ccache)
     set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
     ```
   - This caches object files and can yield massive rebuild speedups when headers or sources haven’t changed.

2. **Switch to Ninja Generator & Parallel Builds**

   - In your build scripts (e.g., `cmake-b-ninja.cmd`), invoke CMake with `-G Ninja`.
   - Build with `cmake --build . --parallel` or `cmake --build . -- -jN` to fully utilize all CPU cores.

3. **Adopt Precompiled Headers (PCH)**

   - With CMake ≥3.16, use `target_precompile_headers()` to precompile your most-included headers (e.g., common library headers). This reduces redundant parsing cost across translation units.

4. **Leverage Unity (Jumbo) Builds**

   - If upgrading to CMake ≥3.27, enable per-target unity builds:
     ```cmake
     set_target_properties(${LIBRARY_NAME} PROPERTIES UNITY_BUILD ON)
     ```
   - Groups multiple sources into fewer translation units, drastically cutting compiler overhead.

5. **Update Minimum CMake Version**

   - Bump `cmake_minimum_required()` to at least 3.27 to unlock built-in PCH and UNITY_BUILD support.

6. **Move from Global Flags to Target-Scoped Settings**

   - Replace direct manipulation of `CMAKE_CXX_FLAGS` with `target_compile_features()`, `target_compile_options()`, and generator expressions for per-target/class settings. This improves CMake’s incremental update performance.

7. **Control Optional Components**

   - Add `option(BUILD_TESTS "Build unit tests" OFF)` and guard `add_subdirectory(tests)`.
   - Disable building examples, docs, or coverage targets by default. Only enable when needed.

8. **Optimize FetchContent Usage**

   - Use pinned, release-tagged dependencies to avoid repeated Git clone overhead.
   - Consider pre-fetching or using system-installed libraries instead of rebuilding third-party code every time.

9. **Curate Warning Flags**

   - On Clang, replace `-Weverything` with a focused set (e.g., `-Wall -Wextra -Wpedantic`) to reduce the number of checks and improve compile speed.

10. **Miscellaneous Tips**
    - Export `CMAKE_EXPORT_COMPILE_COMMANDS=ON` only in dev scenarios (it can slow the main build).
    - For MSVC on Windows, enable `/MP` for multi-processor compilation:
      ```cmake
      add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
      ```
