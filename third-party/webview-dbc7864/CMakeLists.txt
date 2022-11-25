cmake_minimum_required(VERSION 3.1.2)

project(webview)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(webview STATIC src/webview.cpp)

if (WIN32)
    target_compile_options(webview PRIVATE /W4)

    # copy bin dir to lib dir
    execute_process(COMMAND powershell -Command
    "!(Test-Path WebView2.zip) -and (Invoke-WebRequest -Uri https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2 -OutFile WebView2.zip)"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   )

   # unzip WebView2.zip to WebView2



#   execute_process(COMMAND powershell -Command
#       "Expand-Archive -Path WebView2.zip -DestinationPath WebView2"
#       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#   )

   execute_process(COMMAND powershell -Command
       "!(Test-Path WebView2) -and (Expand-Archive -Path WebView2.zip -DestinationPath WebView2)"
       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   )

   target_include_directories(webview PRIVATE ${CMAKE_BINARY_DIR}/WebView2/build/native/include)
   target_link_directories(webview PRIVATE ${CMAKE_BINARY_DIR}/WebView2/build/native/x64)
   target_link_libraries(webview PRIVATE ${CMAKE_BINARY_DIR}/WebView2/build/native/x64/WebView2LoaderStatic.lib)

elseif(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(webview PRIVATE -Wall -Wextra -Werror -pedantic -Wno-unused-lambda-capture)
    endif()

    find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)

    target_link_libraries(webview INTERFACE ${GTK3_LIBRARIES} ${WEBKIT2_LIBRARIES} pthread)
    target_include_directories(webview SYSTEM PUBLIC ${GTK3_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
endif()



target_link_libraries(webview)
