# Task-Scoped Bash Command Wrappers: A Design and Maintenance Manual
2025-06-28

## Introduction

This manual provides a comprehensive guide to designing, creating, and maintaining task-scoped Bash command wrappers. These wrappers are small, specialized scripts that tame large, general-purpose command-line interfaces (CLIs) by exposing only the specific functionality you need for your workflows.

Although the examples below often feature `git` and `ffmpeg`, the pattern is universal. It applies to any CLI or API you use, including but not limited to Terraform, Docker, the Azure CLI, `kubectl`, and ImageMagick. Whenever you find yourself repeatedly typing a complex sequence of flags or performing a manual, error-prone task, consider encapsulating that workflow into a script following the principles outlined here.

## 1. Purpose and Philosophy

The core philosophy behind a task-scoped wrapper is to narrow a heavyweight toolchain to the small slice you actually use. Each script should adhere to the following principles:

*   **Define a Single Domain:** Each wrapper should correspond to one logical domain, such as Git operations, video manipulation, or cloud infrastructure management.
*   **Expose Plain-English Commands:** Subcommands should be clear, memorable verb phrases (e.g., `diff-from-main`, `compress-gif`) that describe a specific task.
*   **Provide a Single Entry Point:** The wrapper is the single interface for the task. Callers should never need to remember or use the raw flags of the underlying tool.
*   **Validate and Guide:** The script must validate its inputs, provide clear help text, and print helpful error messages to guide the user.

This approach turns complex tools into predictable, self-documenting building blocks that can be version-controlled, peer-reviewed, and safely integrated into automated pipelines.

## 2. Anatomy of a Wrapper Script

A well-structured wrapper script consists of several distinct parts. Always start your script with a shebang and strict mode settings.

```bash
#!/bin/bash
set -euo pipefail
```

### 2.1. Command Constants

Define a read-only variable for each subcommand. This practice prevents typos and makes the code easier to maintain, as the command name is defined in exactly one place.

```bash
readonly COMMAND_DIFF_FROM_MAIN="diff-from-main"
readonly COMMAND_HELP="help"
```

**Guidelines:**

*   Keep constant names in `kebab-case` for readability and consistency.
*   Sort the constants alphabetically to minimize merge conflicts in version control.

### 2.2. Help Message

A dedicated `usage` function is the best way to provide help. Use a "here document" (`cat <<EOF`) to create a multi-line string for the help text.

```bash
usage() {
    cat <<EOF
Usage:
   ff.sh <command> [options]

Description:
   A wrapper for common FFmpeg video and GIF operations.

Commands:
    ${COMMAND_HELP}
      Shows this help message.

    ${COMMAND_CONVERT_WEBM} --file-path <path>
      Converts an input video to the WebM format.
      Example:
         ff.sh convert-webm --file-path "input.mov"
EOF
}
```

**Guidelines:**

*   Wrap the help text at 80 columns for optimal readability in standard terminals.
*   Provide at least one concrete, runnable example for every subcommand.

### 2.3. Utility Functions

Delegate all significant work to private functions. By convention, private function names start with an underscore (`_`).

```bash
_convert_webm() {
    local file_path=""

    # Parse named arguments for this specific function
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --file-path)
                file_path="$2"
                shift 2
                ;;
            *)
                echo "Error: Unknown argument '$1' for convert-webm" >&2
                return 1
                ;;
        esac
    done

    if [[ -z "$file_path" ]]; then
        echo "Error: --file-path is required for convert-webm." >&2
        return 1
    fi

    if [[ ! -f "$file_path" ]]; then
        echo "Error: File not found at '$file_path'" >&2
        return 1
    fi

    local output_path="${file_path%.*}.webm"
    ffmpeg -i "$file_path" -c:v libvpx -c:a libvorbis "$output_path"
}
```

**Guidelines:**

*   Use `local` for all variables inside functions to avoid polluting the global scope.
*   Validate parameters at the beginning of the function and `return 1` on failure.
*   Print all error messages to standard error (`>&2`).

### 2.4. Dispatcher `case` Statement

A `case` statement is the script's central router. It inspects the first command-line argument, matches it to a known command, and calls the corresponding function.

```bash
main() {
    # The first argument is the command. Default to "help" if not provided.
    local command="${1-help}"
    shift || true # Shift arguments to the left.

    case "${command}" in
        "${COMMAND_HELP}")
            usage
            ;;
        "${COMMAND_CONVERT_WEBM}")
            _convert_webm "$@" # Pass remaining arguments to the function
            ;;
        *)
            echo "Error: Unknown command '${command}'" >&2
            usage
            exit 1
            ;;
    esac
}

# Execute the main function with all script arguments
main "$@"
```

### 2.5. Post-Execution Footer

A `trap` on `EXIT` can execute code when the script finishes, which is useful for logging or cleanup.

```bash
# Place this near the top of the script
trap 'echo "Done: $(date -u --iso-8601=seconds)"' EXIT
```

## 3. Creating a New Wrapper Script

1.  **Choose a Name:** Pick a concise, descriptive file name (e.g., `aws.sh`, `docker.sh`).
2.  **Copy Boilerplate:** Clone the file structure from the most mature existing script.
3.  **Make it Executable:** Run `chmod +x your-script.sh`.
4.  **Customize:** Remove unrelated constants, functions, and `case` entries.
5.  **Write Help Text:** Fill in the `usage` function with a clear description and examples.
6.  **Commit:** Make an initial commit with a passing test to establish a valid baseline.

## 4. Adding a New Subcommand

1.  **Define the Command:** Choose a clear, user-facing verb phrase (e.g., `compress-gif`).
2.  **Add a Constant:** Add a new `readonly COMMAND_...` constant, maintaining alphabetical order.
3.  **Update Help:** Extend the `usage` function with the new command, its parameters, and an example.
4.  **Implement the Function:** Write a dedicated `_my_new_command` function with argument parsing and validation.
5.  **Wire the `case`:** Add a new entry to the `case` statement that calls the function.
6.  **Verify:** Run the script with the `help` command to ensure the new entry renders correctly.
7.  **Test:** Add a test using a framework like `bats-core` or a simple shell script that asserts a successful exit code (`0`).
8.  **Commit:** Commit the changes with a descriptive message, e.g., `feat(ff.sh): add compress-gif subcommand`.

## 5. Maintenance Guidelines

*   **Keep it Concise:** Aim to keep each script under 500 lines. If a script grows too large, split it into more specialized wrappers.
*   **Refactor Shared Code:** If you duplicate helper functions, refactor them into a common script that can be `source`d by the wrappers.
*   **Log Breaking Changes:** Document any backward-incompatible changes in a `CHANGELOG.md` file.
*   **Automate Quality Checks:** Enforce a pre-commit git hook that runs `shellcheck` for static analysis and your test suite.
*   **Conduct Peer Reviews:** Use a checklist for pull requests: constants added, help updated, error paths return non-zero, and tests cover both success and failure cases.
*   **Tag Releases:** Use version control tags for releases and pin automation pipelines to specific tags for stable, reproducible builds.

## 6. Version Control and Distribution

*   **Centralize Scripts:** Store all wrapper scripts in a dedicated `scripts/` or `bin/` directory at the root of the repository.
*   **Add to PATH:** For easy distribution, users can add this directory to their shell's `$PATH` environment variable.
*   **Publish Documentation:** A CI/CD job can extract the `usage()` text from each script and publish it as browsable documentation.

## 7. Example Workflow: Adding `convert-webm` to `ff.sh`

This example demonstrates the end-to-end process of adding a new subcommand.

**1. Define the Constant**
```bash
readonly COMMAND_CONVERT_WEBM="convert-webm"
```

**2. Extend the Help Message**
```bash
# Inside the usage() function's here-doc
    ${COMMAND_CONVERT_WEBM} --file-path <path>
      Converts an input video to WebM format.
      Example:
         ff.sh convert-webm --file-path "input.mp4"
```

**3. Implement the Function**
```bash
_convert_webm() {
    # ... (implementation from section 2.3) ...
}
```

**4. Add to the `case` Statement**
```bash
# Inside the main() function's case statement
        "${COMMAND_CONVERT_WEBM}")
            _convert_webm "$@"
            ;;
```

**5. Update Tests and Commit**
Finally, add a `bats-core` test and commit the changes.

## 8. Development Checklist

Use this checklist when adding a new command or creating a new wrapper.

*   [ ] Constant added alphabetically
*   [ ] `usage()` function updated with description and example
*   [ ] Dedicated `_function` created with argument parsing and validation
*   [ ] `case` statement wired to the new function
*   [ ] Unit test written for the success case
*   [ ] `shellcheck` passes without errors
*   [ ] `CHANGELOG.md` entry added if applicable

## Inspiration

The `kebab-case` command naming convention is inspired by the style of interactive commands in Emacs Lisp. As in Emacs, this pattern empowers you to coin your own verbs and bind them to any sequence of actions, effectively turning your personal workflows into first-class, repeatable commands.



## Example 

```sh
#!/bin/bash
# ANNOTATED EXAMPLE OF A TASK-SCOPED BASH WRAPPER
#
# This script demonstrates the design pattern described in the manual.
# It serves as a template for creating new, robust command wrappers.

# Set strict mode.
# -e: exit immediately if a command exits with a non-zero status.
# -u: treat unset variables as an error when substituting.
# -o pipefail: the return value of a pipeline is the status of the last
#              command to exit with a non-zero status, or zero if no
#              command exited with a non-zero status.
set -euo pipefail

# Section 2.5: Post-Execution Footer (via trap)
# =============================================
# A 'trap' on EXIT ensures this command runs when the script finishes,
# regardless of whether it succeeded or failed. This is ideal for
# logging or cleanup actions.
trap 'echo "Done: $(date -u --iso-8601=seconds)"' EXIT

# Section 2.1: Command Constants
# ==============================
# Define read-only variables for each subcommand. This avoids "magic strings"
# and makes the code easier to refactor and maintain. They are sorted
# alphabetically to reduce version control merge conflicts.
readonly COMMAND_DO_SOMETHING="do-something"
readonly COMMAND_HELP="help"

# Section 2.2: Help Message
# =========================
# A dedicated 'usage' function provides help. Using a "here document"
# (cat <<EOF) is the standard way to create multi-line strings in Bash.
usage() {
    cat <<EOF
Usage:
   examples.sh <command> [options]
   aww run examples <command> [options]

Description:
   An example script demonstrating the task-scoped wrapper pattern in Bash.

Commands:
    ${COMMAND_HELP}
      Shows this help message.

    ${COMMAND_DO_SOMETHING} --file-path <path> --message <text>
      A sample command that demonstrates calling a utility function.
      Example:
         examples.sh do-something --file-path "./my-file.txt" --message "Hello World"

EOF
}

# Section 2.3: Utility Functions
# ==============================
# All real work is delegated to private functions. By convention, private
# function names start with an underscore (_). They parse their own arguments.
_do_something() {
    # Use 'local' to keep variables scoped to the function.
    local file_path=""
    local message=""

    # A standard loop for parsing named arguments.
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --file-path)
                file_path="$2"
                shift 2 # Move past the flag and its value
                ;;
            --message)
                message="$2"
                shift 2 # Move past the flag and its value
                ;;
            *)
                echo "Error: Unknown argument '$1' for do-something" >&2
                return 1
                ;;
        esac
    done

    # Validate that required arguments were provided.
    if [[ -z "$file_path" || -z "$message" ]]; then
        echo "Error: --file-path and --message are required for do-something." >&2
        usage >&2 # Print usage to stderr as well
        return 1
    fi

    echo "Simulating action..."
    echo "  - FilePath: ${file_path}"
    echo "  - Message: ${message}"
    # In a real script, you would perform file operations, API calls, etc.
    # For example: echo "${message}" >> "${file_path}"
    echo "Simulation complete."
}

# Section 2.4: Dispatcher `case` Statement
# ========================================
# The main function acts as the script's router. It inspects the first
# positional parameter to decide which command to run.
main() {
    # Default to the 'help' command if no command is provided.
    local command="${1-help}"
    # Shift arguments to the left, so that sub-functions don't see the
    # main command but get all the other arguments. '|| true' prevents
    # an error if no arguments were passed.
    shift || true

    case "${command}" in
        "${COMMAND_HELP}")
            usage
            ;;
        "${COMMAND_DO_SOMETHING}")
            # Pass all remaining arguments ("$@") to the utility function.
            _do_something "$@"
            ;;
        *)
            # Handle unknown commands, print an error to stderr, show usage,
            # and exit with a non-zero status code to signal failure.
            echo "Error: Unknown command '${command}'" >&2
            usage >&2
            exit 1
            ;;
    esac
}

# Execute the main function, passing all script arguments to it.
# This is the entry point of the script.
main "$@"

```



