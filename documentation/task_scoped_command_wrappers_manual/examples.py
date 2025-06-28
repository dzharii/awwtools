#!/usr/bin/env python3
# ANNOTATED EXAMPLE OF A TASK-SCOPED PYTHON WRAPPER
#
# This script demonstrates the design pattern described in the Python manual.
# It serves as a template for creating new, robust command wrappers using
# Python's standard `argparse` library.

# Section 2.1: Shebang and Imports
# ================================
# Standard library imports. `argparse` for CLI parsing, `sys` for stderr and
# exit codes, `pathlib` for modern path manipulation, and `datetime` for the footer.
import argparse
import sys
from pathlib import Path
from datetime import datetime

# Section 2.3: Utility Functions
# ==============================
# All real work is delegated to dedicated functions. By convention, functions
# not intended to be called from other modules are prefixed with an underscore.
# They receive the `args` object from argparse, which contains the parsed
# command-line arguments.
def _do_something(args: argparse.Namespace) -> None:
    """
    Handles the logic for the 'do-something' command.
    This function receives an object with the parsed arguments.
    """
    # Type hints make the code clearer and allow static analysis.
    file_path: Path = args.file_path
    message: str = args.message

    # argparse with `required=True` handles validation for missing arguments.
    # We can add our own custom validation logic here.
    print("Simulating action...")
    print(f"  - FilePath: {file_path}")
    print(f"  - Message: {message}")
    # In a real script, you would perform file operations, API calls, etc.
    # For example: file_path.write_text(message)
    print("Simulation complete.")


# Section 2.2: Argument Parser Setup
# ==================================
# This function builds the command-line interface using argparse.
# Using a dedicated function keeps the main execution block clean.
def create_parser() -> argparse.ArgumentParser:
    """Creates and configures the argument parser."""
    # The main parser object. The description is shown in the help message.
    parser = argparse.ArgumentParser(
        description="An example script demonstrating the task-scoped wrapper pattern in Python.",
        # RawTextHelpFormatter preserves newlines and spacing in help text.
        formatter_class=argparse.RawTextHelpFormatter,
    )

    # Subparsers are the standard way to implement sub-commands like 'git pull' or 'docker run'.
    # The 'dest="command"' attribute will store which subcommand was used.
    subparsers = parser.add_subparsers(
        dest="command",
        required=True,
        help="Available commands"
    )

    # Create a sub-parser for the 'do-something' command.
    # The help text appears in the main help listing.
    parser_do_something = subparsers.add_parser(
        "do-something",
        help="A sample command that demonstrates calling a utility function.",
        description="This is the detailed description for the 'do-something' command, shown when running 'examples.py do-something --help'."
    )
    # Add arguments for this specific subcommand.
    parser_do_something.add_argument(
        "--file-path",
        type=Path,  # Use pathlib.Path for automatic path object creation.
        required=True,
        help="The path to the target file."
    )
    parser_do_something.add_argument(
        "--message",
        type=str,
        required=True,
        help="The message to write."
    )
    # Associate the utility function with this sub-parser. When this command
    # is chosen, the `func` attribute on the args object will point to `_do_something`.
    parser_do_something.set_defaults(func=_do_something)

    return parser


# Section 2.4: Main Execution Block
# =================================
# The main() function orchestrates the script's execution.
def main() -> None:
    """Main entry point for the script."""
    parser = create_parser()
    args = parser.parse_args()

    # This is the dispatcher. We call the function that was associated with the
    # chosen subcommand via `set_defaults(func=...)`.
    args.func(args)

# This is the standard Python entry point. It ensures the code inside only
# runs when the script is executed directly.
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # Catch-all for unexpected errors for a more graceful exit.
        print(f"An unexpected error occurred: {e}", file=sys.stderr)
        sys.exit(1)
    finally:
        # Section 2.6: Post-Execution Footer
        # ==================================
        # A timestamp at the end provides a clear signal that the script
        # completed and can be useful for logging and auditing.
        print(f"Done: {datetime.now().isoformat()}")
