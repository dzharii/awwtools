# Developer Setup Guide for awwtools

This guide provides instructions for setting up the awwtools development environment on Windows and Linux platforms. awwtools is a C++ toolkit designed to simplify productivity workflows across different operating systems.

# Quick Setup

### Windows (PowerShell)

```powershell
# Install required tools
winget install -e --id Kitware.CMake
winget install -e --id Git.Git
# Visual Studio 2022 version 17: https://aka.ms/vs/17/release/vs_community.exe
# During installation select "Desktop development with C++"
## ALTERNATIVE:
# Visual Studio Build Tools: https://aka.ms/vs/17/release/vs_buildtools.exe
# During installation select "Desktop development with C++"


# Clone and build the project
git clone https://github.com/dzharii/awwtools.git
cd awwtools
cmake-b.cmd
```

### Linux (Ubuntu)

```bash
## cmake alternative:
# sudo snap install cmake --classic
# Install all dependencies
sudo apt update && sudo apt install -y build-essential git cmake libgtk-3-dev libwebkit2gtk-4.0-dev

# Clone and build the project
git clone https://github.com/dzharii/awwtools.git
cd awwtools
cmake-b.sh
```

## Running the Applications

awwtools consists of multiple command-line utilities. After building, the executables can be found in the build directory.

### Command-line Tools

The main tools include:

- `aww-create`: Creates files from templates
- `aww-open`: Cross-platform file opener
- `aww-git-open`: Opens repositories in browser
- `aww-guid`: Generates GUIDs
- `aww-tag`: Tagging utility
- `aww-toast`: Desktop notifications
- `aww-run`: Script execution utility

#### Windows

```powershell
# Running from build directory
.\cmake-build\Debug\aww-create.exe [arguments]
.\cmake-build\Debug\aww-git-open.exe [arguments]
```

#### Linux

```bash
# Running from build directory
./cmake-build/aww-create [arguments]
./cmake-build/aww-git-open [arguments]
```

### Using the Launcher

The project includes an `aww` launcher that provides a unified interface:

```bash
# Examples
aww create filename.txt
aww git open [optional file or folder path]
```

## Project Structure

```
awwtools/
├── app/                    # Application entry points
├── cmake/                  # CMake modules and configurations
├── docs/                   # Documentation and web pages
├── documentation/          # Additional documentation files
├── include/                # Public header files
│   └── internal/           # Internal header files
├── src/                    # Source code implementation
│   └── internal/           # Internal implementations
└── tests/                  # Unit tests
```

### Key Components

- **aww-common**: Core utilities shared across tools
- **aww-create**: File creation utility with templating
- **aww-git-open**: Utility to open git repositories in browsers
- **aww-open**: Cross-platform file opener
- **aww-toast**: Desktop notification system
- **aww-run**: Script execution utility

## Troubleshooting

### Common Issues on Windows

1. **Missing Visual Studio Components**:

   - Solution: In Visual Studio Installer, modify your installation to include "Desktop development with C++" and "C++ CMake tools for Windows"

2. **Build Errors with WinToast**:
   - Solution: Ensure you're using a recent version of Visual Studio with Windows SDK

### Common Issues on Linux

1. **GTK or WebKit2GTK Not Found**:
   Solution: Install the required development packages:

```bash
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev
```

2. **CMake Cannot Find Compiler**:
   Solution: Install build-essential package:

```bash
sudo apt install build-essential
```

## Maintainability

### Updating Dependencies

The project uses CMake's FetchContent to manage most dependencies. To update a dependency:

1. Locate the FetchContent_Declare entry in CMakeLists.txt
2. Update the GIT_TAG to the desired version
3. Rebuild the project

Example:

```cmake
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2  # Change this to update the version
)
```

### Adding New Modules

To add a new tool to the awwtools collection:

1. Create header files in `include/internal/`
2. Implement the tool in internal
3. Create an entry point in app
4. Add source files to the SOURCES list in CMakeLists.txt
5. Add tests in the tests directory

### Cross-Platform Considerations

When adding new functionality:

Use conditional compilation for platform-specific code:

```cpp
#ifdef _WIN32
// Windows-specific code
#elif defined(__linux__)
// Linux-specific code
#else
// Generic implementation
#endif
```

For platform-specific implementations:
Use separate files (e.g., aww-common-windows.cpp, aww-common-linux.cpp)
Use appropriate CMake conditionals to include them:

```cmake
if(WIN32)
    list(APPEND SOURCES src/platform-windows.cpp)
elseif(UNIX)
    list(APPEND SOURCES src/platform-linux.cpp)
endif()
```

When using system APIs, provide abstraction layers that can be implemented differently per platform
