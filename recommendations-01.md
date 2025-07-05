# How to improve compile time

2025-05-13

Prompt:

> I want you to analyze the entire codebase and find the way to optimize build time. look into Cmake files, suggest what I can do to make the build more optimal, more faster. use modern best practices and at the end only edit recommendations.md and put all recommendations there. start from the most impactful and add all.

# LLM Recommendations on how to improve compile and build time:

# CMake Build Optimization Recommendations

Here are recommendations to optimize the CMake build times for the AwwTools project, ordered roughly by potential impact:

1.  **Enable Caching for `FetchContent`:**

    - **Problem:** `FetchContent` re-downloads dependencies on every configure if not explicitly cached.
    - **Solution:** Set `FETCHCONTENT_BASE_DIR` to a persistent cache directory _before_ any `FetchContent_Declare` calls. This tells CMake to store and reuse downloaded content.
      ```cmake
      # Near the top of your CMakeLists.txt, after project()
      set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps_cache" CACHE PATH "Directory to cache FetchContent downloads")
      message(STATUS "FetchContent cache directory: ${FETCHCONTENT_BASE_DIR}") # Optional: for verification
      ```
    - **Impact:** High. Drastically reduces reconfiguration time when dependencies haven't changed.

2.  **Use Precompiled Headers (PCH):**

    - **Problem:** Common headers (e.g., from your own project like `aww-common.hpp`, standard library headers like `<vector>`, `<string>`, or third-party library headers like `spdlog.h`, `fmt/core.h`) are re-parsed and re-compiled for every source file that includes them.
    - **Solution:** Create a central precompiled header file (e.g., `src/pch.h`) that includes these common, relatively stable headers. Then, instruct CMake to use this PCH for your targets.

      ```cmake
      # Example src/pch.h:
      // #pragma once // Or use traditional include guards
      // #ifndef AWW_PCH_H
      // #define AWW_PCH_H
      //
      // // Standard Library
      // #include <iostream>
      // #include <string>
      // #include <vector>
      // #include <filesystem>
      // #include <algorithm>
      //
      // // Third-party
      // #include "fmt/core.h"
      // #include "spdlog/spdlog.h"
      // #include "nlohmann/json.hpp"
      // #include "clip.h"
      //
      // // Project-specific common headers
      // #include "aww-common.hpp"
      //
      // #endif // AWW_PCH_H

      # In CMakeLists.txt, after defining LIBRARY_NAME:
      target_precompile_headers(${LIBRARY_NAME}
          PUBLIC
              $<BUILD_INTERFACE:src/pch.h>
      )
      # You might also apply this to your executables if they don't link the library
      # or have significant independent common includes.
      # For example, for the 'aww' executable:
      # target_precompile_headers(aww PRIVATE src/pch.h)
      ```

      Ensure that your source files include this `pch.h` as the very first include (or after `#pragma once` if used within the PCH itself).

    - **Impact:** High. Significantly speeds up compilation, especially for larger projects.

3.  **Refactor Repetitive Target Definitions with a Function:**

    - **Problem:** The `CMakeLists.txt` has a lot of repetition for adding each `aww-` tool executable (e.g., `add_executable`, `target_link_libraries`, `target_set_warnings`, `target_enable_lto`).
    - **Solution:** Create a CMake function to encapsulate this common logic.

      ```cmake
      # Define this function before you start adding your executables
      function(add_aww_executable tool_name)
          set(tool_source_file "app/${tool_name}.cpp")
          add_executable(${tool_name} ${tool_source_file})
          target_link_libraries(${tool_name} PRIVATE ${LIBRARY_NAME})

          # Handle specific linking needs, e.g., for aww-tee
          if(${tool_name} STREQUAL "aww-tee")
              target_link_libraries(${tool_name} PRIVATE webview::core)
          endif()

          target_set_warnings(${tool_name} ENABLE ALL AS_ERROR ALL DISABLE Annoying)
          target_enable_lto(${tool_name} optimized) # Assumes 'optimized' correctly handles build types

          # Add to the list of projects for common properties
          list(APPEND AWW_TOOL_TARGETS ${tool_name})
          set(AWW_TOOL_TARGETS ${AWW_TOOL_TARGETS} PARENT_SCOPE) # Propagate list to parent scope
      endfunction()

      # Then, use the function:
      # add_aww_executable(aww)
      # add_aww_executable(aww-git-open)
      # ... and so on for all tools

      # Your PROJECTS list would then become:
      # set(PROJECTS ${LIBRARY_NAME} ${AWW_TOOL_TARGETS})
      ```

    - **Impact:** Medium. Improves maintainability, readability, and reduces the chance of errors when adding new tools. Does not directly speed up the build but makes management easier.

4.  **Use Ninja Build System Consistently:**

    - **Problem:** While you have `cmake-b-ninja.cmd`, ensuring it's the default or highly recommended generator can improve build parallelism and incremental build speed over traditional Makefiles or MSBuild.
    - **Solution:** Continue to promote and use Ninja. Document it as the preferred way to build.
      ```bash
      # For developers:
      cmake -G Ninja -S . -B build
      cmake --build build
      ```
    - **Impact:** Medium. Can provide noticeable speedups, especially for incremental builds and on multi-core systems.

5.  **Ensure Link Time Optimization (LTO) is Conditional:**

    - **Problem:** LTO (`target_enable_lto`) improves runtime performance but significantly increases link times, which is undesirable for debug/development builds.
    - **Solution:** Your use of `target_enable_lto(... optimized)` likely relies on your `cmake/LTO.cmake` module to correctly apply LTO only for release-like configurations. Double-check that `find_lto` and `target_enable_lto` in your `LTO.cmake` module correctly restrict LTO to `Release` and `RelWithDebInfo` build types. If not, you can make it more explicit:
      ```cmake
      # Inside your add_aww_executable function or where target_enable_lto is called:
      if(CMAKE_BUILD_TYPE MATCHES "^(Release|RelWithDebInfo)$")
          target_enable_lto(${tool_name} optimized) # Or directly call the LTO setup
      endif()
      # Or, ensure your ENABLE_LTO option is respected and defaults to OFF for Debug.
      ```
    - **Impact:** Medium. Reduces link times significantly during development builds.

6.  **Build Only Necessary Targets During Development:**

    - **Problem:** Building all executables and the library by default (`cmake --build .`) can be slow if a developer is only working on a specific tool or the library.
    - **Solution:** Encourage developers to build specific targets.
      ```bash
      cmake --build . --target aww-my-specific-tool
      cmake --build . --target ${LIBRARY_NAME}
      ```
      Consider if all executables _need_ to be part of the default `ALL` target. For very frequently iterated tools, this is fine, but for less common ones, making them non-default could speed up the common "build all" command for those who use it.
    - **Impact:** Medium. Saves time by not building unused components during iterative development.

7.  **Consider Unity (Jumbo) Builds (with caution):**

    - **Problem:** Compiling many small source files individually has overhead (opening files, invoking compiler, etc.).
    - **Solution:** CMake supports unity builds, where multiple `.cpp` files are grouped and compiled as one larger unit.

      ```cmake
      # Potentially set globally or per target
      set(CMAKE_UNITY_BUILD ON)
      # You can control batch sizes etc.
      # set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

      # Apply to relevant targets, e.g., your main library or all projects
      # set_target_properties(${LIBRARY_NAME} PROPERTIES UNITY_BUILD ON)
      # For all projects in your list:
      # foreach(proj ${PROJECTS})
      #    set_target_properties(${proj} PROPERTIES UNITY_BUILD ON)
      # endforeach()
      ```

    - **Caution:** Unity builds can sometimes hide include errors (e.g., missing includes in a `.cpp` file that happen to be included by another file in the same unity blob) and can make debugging slightly more complex. They can also lead to very large object files that might hit compiler limits or increase memory usage during compilation. Test thoroughly.
    - **Impact:** Potentially High, but with caveats. Can significantly reduce build times if applicable and if the downsides are manageable.

8.  **Analyze Build Times with Profiling Tools:**

    - **Problem:** To find the _actual_ bottlenecks, you need to measure.
    - **Solution:**
      - **Compiler Timings:**
        - Clang/GCC: Use the `-ftime-trace` flag. This generates a JSON file that can be visualized in `chrome://tracing` to see where compilation time is spent.
        - MSVC: Use `/Bt+` (linker) and `/ftime+` (compiler) or the BuildInsights tools in Visual Studio.
      - **CMake Configure Time:**
        ```bash
        cmake --profiler-format=google-trace --profiler-output=cmake_prof.json .
        ```
        Then analyze `cmake_prof.json` in `chrome://tracing`.
    - **Impact:** Meta-level. Essential for guiding further optimization efforts by identifying the slowest parts of the build.

9.  **Review and Optimize Third-Party Library Builds via `FetchContent`:**

    - **Problem:** `FetchContent` builds dependencies as part of your project. If these dependencies build their own tests, examples, or are built in debug mode when not needed, it adds to your build time.
    - **Solution:**

      - **`spdlog` & `fmt`:** You correctly set `SPDLOG_FMT_EXTERNAL ON` and `SPDLOG_BUILD_SHARED OFF`. This is good.
      - **`nlohmann_json`:** You set `JSON_BuildTests OFF CACHE INTERNAL ""`. This is good.
      - **`clip` & `wintoastlib`:** These are added via `add_subdirectory`. Check their `CMakeLists.txt` files for options to disable tests or examples (e.g., `BUILD_TESTING`, `CLIP_BUILD_EXAMPLES`, `WINTOAST_BUILD_EXAMPLES`). You can set these options _before_ `add_subdirectory` or `FetchContent_MakeAvailable`.

        ```cmake
        # Before FetchContent_MakeAvailable(clip) or add_subdirectory for clip
        # set(CLIP_BUILD_TESTS OFF CACHE BOOL "Disable clip tests")
        # set(CLIP_BUILD_EXAMPLES OFF CACHE BOOL "Disable clip examples")

        # Before FetchContent_MakeAvailable(wintoastlib) or add_subdirectory for wintoastlib
        # set(WINTOAST_BUILD_EXAMPLES OFF CACHE BOOL "Disable wintoast examples")
        ```

      - **`awwlib` & `webview`:** For `FetchContent`-ed libraries, you can pass `CMAKE_ARGS` to `FetchContent_Declare` or set cache variables before `FetchContent_MakeAvailable` to configure their build. For example, to disable tests if `awwlib` has an option like `AWWLIB_BUILD_TESTS`:
        ```cmake
        # FetchContent_Declare(
        #     awwlib
        #     GIT_REPOSITORY https://github.com/dzharii/awwlib-cpp.git
        #     GIT_TAG        dmytro_zharii/2024-10-13-dev
        #     GIT_SHALLOW TRUE
        #     CMAKE_ARGS -DAWWLIB_BUILD_TESTS=OFF
        # )
        # Or:
        # set(AWWLIB_BUILD_TESTS OFF CACHE BOOL "")
        # FetchContent_MakeAvailable(awwlib)
        ```
        Investigate the CMake options provided by `awwlib` and `webview`.

    - **Impact:** Medium. Reducing unnecessary work in dependencies can noticeably speed up the overall build.

10. **`CMAKE_CXX_STANDARD` and `CXX_EXTENSIONS`:**

    - **Current Status:** You correctly set `CXX_STANDARD 20`, `CXX_STANDARD_REQUIRED YES`, and `CXX_EXTENSIONS NO`.
    - **Recommendation:** This is good practice and clearly defines compiler expectations. No change needed here.
    - **Impact:** N/A (already good).

11. **Consider `CMAKE_EXPORT_COMPILE_COMMANDS`:**
    - **Problem:** Many tools (linters, language servers) use `compile_commands.json`.
    - **Solution:** Add `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` to generate this file.
    - **Impact:** Low on build time itself, but high for developer tooling integration.

Remember to apply these changes incrementally and measure the impact on build times using a consistent methodology to see what yields the best results for your specific project and environment.
