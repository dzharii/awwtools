cmake_minimum_required(VERSION 3.1.2)

project(webview)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(webview STATIC src/webview.cpp)

if (WIN32)
    target_compile_options(webview PRIVATE /W4)
    
    find_program(NUGET_EXE NAMES nuget)
	if(NOT NUGET_EXE)
		message("NUGET.EXE not found.")
		message(FATAL_ERROR "Please install this executable, and run CMake again.")
	endif()

	exec_program(${NUGET_EXE}
    	ARGS install "Microsoft.Web.WebView2" -Version 1.0.865-prerelease -ExcludeVersion -OutputDirectory ${CMAKE_BINARY_DIR}/packages)
	exec_program(${NUGET_EXE}
    	ARGS install "Microsoft.Windows.ImplementationLibrary" -ExcludeVersion -OutputDirectory ${CMAKE_BINARY_DIR}/packages)

    target_link_libraries(webview INTERFACE Version.lib Shlwapi.lib ${CMAKE_BINARY_DIR}/packages/Microsoft.Web.WebView2/build/native/x64/WebView2LoaderStatic.lib ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.ImplementationLibrary/build/native/Microsoft.Windows.ImplementationLibrary.targets)
    target_include_directories(webview SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/packages/Microsoft.Web.WebView2/build/native/include ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.ImplementationLibrary/include)
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
