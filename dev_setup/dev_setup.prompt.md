Analyze this C++ project thoroughly, ensuring that every file and dependency is examined -- do not skip any files, even seemingly unused or test files. The project is primarily a command-line tool but also includes GUI components. It uses CMake as the build system and has third-party dependencies.

Your task is to produce a comprehensive developer setup guide in Markdown format. This guide must be tailored to support **cross-platform development** on both **Windows** and **Linux**.

Specifically, the documentation should include:

1. **Environment Requirements**:

   - List all required tools (compilers, CMake, package managers).
   - Include instructions to install an appropriate C++ development environment (e.g., MSVC or MinGW on Windows, GCC/Clang on Linux).

2. **Dependency Setup**:

   - Detect and list all third-party libraries.
   - Include setup steps for each dependency (installation, paths, etc.), including package managers like `vcpkg` or system-specific alternatives.

3. **Project Initialization Instructions**:

   - Step-by-step guide to clone and initialize the project.
   - CMake configuration steps per OS.
   - Build instructions and tips for common build configurations (Debug, Release).

4. **Running the Application**:

   - How to run the command-line interface and GUI.
   - List any runtime dependencies or required environment variables.

5. **Troubleshooting and Tips**:

   - Known platform-specific issues and their solutions.
   - Common mistakes or misconfigurations to avoid.

6. **Structure Overview**:

   - A brief overview of the project’s directory structure and purpose of major components.

7. **Maintainability**:
   - Steps to update dependencies.
   - Guidelines to add new modules or support new platforms.

**Output format**: One well-formatted Markdown document (`DEVELOPER_SETUP.md`). Use headers, subheaders, bullet points, and code blocks. Include verified and tested commands wherever possible.

After this step, I will request a shell script for automatic environment setup, so structure the guide to allow that automation easily.

Make sure your analysis is complete and that the documentation fully reflects the actual contents and structure of the project.
