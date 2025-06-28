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
