# Task-Scoped Python Command Wrappers: A Design and Maintenance Manual
2025-06-28

## Introduction

This manual provides a comprehensive guide to designing, creating, and maintaining task-scoped Python command wrappers. These wrappers are small, specialized scripts that tame large, general-purpose command-line interfaces (CLIs) by exposing only the specific functionality you need for your workflows. Python, with its powerful standard library, is an excellent choice for creating robust, readable, and maintainable CLI tools.

Although the examples below often feature `git` and `ffmpeg`, the pattern is universal. It applies to any CLI or API you use, including but not limited to Terraform, Docker, the Azure CLI, `kubectl`, and ImageMagick. Whenever you find yourself repeatedly typing a complex sequence of flags or performing a manual, error-prone task, consider encapsulating that workflow into a script following the principles outlined here.

## 1. Purpose and Philosophy

The core philosophy behind a task-scoped wrapper is to narrow a heavyweight toolchain to the small slice you actually use. Each script should adhere to the following principles:

*   **Define a Single Domain:** Each wrapper should correspond to one logical domain, such as Git operations, video manipulation, or cloud infrastructure management.
*   **Expose Plain-English Commands:** Subcommands should be clear, memorable verb phrases (e.g., `diff-from-main`, `compress-gif`) that describe a specific task.
*   **Provide a Single Entry Point:** The wrapper is the single interface for the task. Callers should never need to remember or use the raw flags of the underlying tool.
*   **Validate and Guide:** The script must validate its inputs, provide clear help text, and print helpful error messages to guide the user.

This approach turns complex tools into predictable, self-documenting building blocks that can be version-controlled, peer-reviewed, and safely integrated into automated pipelines.

## 2. Anatomy of a Wrapper Script

A well-structured Python wrapper script leverages the `argparse` module to handle most of the boilerplate for command-line parsing, help text generation, and input validation.

### 2.1. Shebang and Imports

Start your script with a shebang pointing to `python3` and import necessary libraries. For modern Python, `pathlib` is preferred for filesystem paths.

```python
#!/usr/bin/env python3

import argparse
import sys
import subprocess
from pathlib import Path
from datetime import datetime
```

### 2.2. Argument Parser Setup

The `argparse` module is the heart of the CLI. You create a main parser and then attach "subparsers" for each command. This is the most idiomatic way to handle subcommands in the standard library.

```python
def create_parser() -> argparse.ArgumentParser:
    """Creates and configures the argument parser."""
    parser = argparse.ArgumentParser(
        description="A wrapper for common FFmpeg video and GIF operations.",
        formatter_class=argparse.RawTextHelpFormatter, # Preserves formatting in help text
    )

    # This object will hold a parser for each subcommand
    subparsers = parser.add_subparsers(dest="command", required=True, help="Sub-command help")

    # Sub-parser for the 'convert-webm' command
    parser_convert = subparsers.add_parser(
        "convert-webm",
        help="Converts an input video to the WebM format.",
        description="Converts an input video to WebM format using libvpx/libvorbis.",
    )
    parser_convert.add_argument(
        "--file-path",
        type=Path,
        required=True,
        help="Path to the input video file.",
    )
    # Associate a function with this sub-parser
    parser_convert.set_defaults(func=_convert_webm)

    return parser
```

### 2.3. Utility Functions

Delegate all significant work to dedicated functions. These functions receive the parsed arguments object. By convention, internal or "private" functions start with an underscore (`_`).

```python
def _convert_webm(args: argparse.Namespace) -> None:
    """Handles the logic for the 'convert-webm' command."""
    input_path: Path = args.file_path

    if not input_path.is_file():
        print(f"Error: File not found at '{input_path}'", file=sys.stderr)
        sys.exit(1)

    output_path = input_path.with_suffix(".webm")
    print(f"Converting '{input_path}' to '{output_path}'...")

    command = [
        "ffmpeg",
        "-i", str(input_path),
        "-c:v", "libvpx",
        "-c:a", "libvorbis",
        str(output_path),
    ]

    # Use subprocess.run for executing external commands
    result = subprocess.run(command, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Error during conversion:\n{result.stderr}", file=sys.stderr)
        sys.exit(1)

    print("Conversion complete.")
```

### 2.4. Main Execution Block

The standard `if __name__ == "__main__":` block serves as the script's entry point. It parses the arguments and calls the function associated with the chosen subcommand.

```python
def main() -> None:
    """Main entry point for the script."""
    parser = create_parser()
    args = parser.parse_args()

    # Call the function associated with the chosen subcommand
    args.func(args)

    print(f"Done: {datetime.now().isoformat()}")

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"An unexpected error occurred: {e}", file=sys.stderr)
        sys.exit(1)
```

## 3. Creating a New Wrapper Script

1.  **Choose a Name:** Pick a concise, descriptive file name (e.g., `aws.py`, `docker.py`).
2.  **Copy Boilerplate:** Clone the file structure from the most mature existing script.
3.  **Make it Executable:** Run `chmod +x your-script.py`.
4.  **Customize:** Remove unrelated subparsers and utility functions.
5.  **Write Help Text:** Update the `description` and `help` text in the `argparse` setup.
6.  **Commit:** Make an initial commit with a passing `pytest` smoke test.

## 4. Adding a New Subcommand

1.  **Define the Command:** Choose a clear, user-facing verb phrase (e.g., `compress-gif`).
2.  **Add a Subparser:** In `create_parser()`, add a new `subparsers.add_parser(...)` for the command.
3.  **Define Arguments:** Use `add_argument()` to define the flags and options for the new command.
4.  **Implement the Function:** Write a new `_my_new_command(args)` function to handle the logic.
5.  **Wire the Function:** Use `set_defaults(func=_my_new_command)` on the new subparser.
6.  **Verify:** Run the script with `--help` and `my-new-command --help` to check the generated help text.
7.  **Test:** Add a `pytest` test that executes the command and asserts a successful outcome.
8.  **Commit:** Commit the changes with a descriptive message, e.g., `feat(ff.py): add compress-gif subcommand`.

## 5. Maintenance Guidelines

*   **Keep it Concise:** Aim to keep each script focused. If it grows too complex, consider splitting it or refactoring it into a proper Python package.
*   **Refactor Shared Code:** If you duplicate helper functions, refactor them into a common `utils.py` module that the scripts can import.
*   **Log Breaking Changes:** Document any backward-incompatible changes in `CHANGELOG.md`.
*   **Automate Quality Checks:** Enforce a pre-commit git hook that runs `black` for formatting, `ruff` or `pylint` for linting, and `pytest` for tests.
*   **Conduct Peer Reviews:** Use a checklist for pull requests: help text is clear, arguments are correctly typed, error paths exit non-zero, and tests cover key cases.
*   **Tag Releases:** Use version control tags for releases and pin automation pipelines to specific tags for stable, reproducible builds.

## 6. Version Control and Distribution

*   **Centralize Scripts:** Store scripts in a `scripts/` or `bin/` directory.
*   **Package as a CLI Tool:** For wider distribution, package the script as a proper Python CLI tool using a `pyproject.toml` file. This allows installation via `pip` and makes the script available in the user's `PATH`.
*   **Publish Documentation:** Tools like Sphinx can automatically generate HTML documentation from your script's docstrings.

## 7. Example Workflow: Adding `convert-webm` to `ff.py`

This example was implemented in the "Anatomy of a Wrapper Script" section, demonstrating the end-to-end process.

## 8. Development Checklist

Use this checklist when adding a new command or creating a new wrapper.

*   [ ] Subparser added in `create_parser()`
*   [ ] Help text (`help`, `description`) is clear and helpful
*   [ ] Arguments added with correct types and requirements
*   [ ] Dedicated `_function(args)` created with type hints
*   [ ] `set_defaults(func=...)` wired to the new function
*   [ ] Unit test written using `pytest`
*   [ ] Linter (`ruff`, `pylint`) and formatter (`black`) pass
*   [ ] `CHANGELOG.md` entry added if applicable

## Inspiration

While the `kebab-case` command names are inspired by other CLIs, Python's `argparse` provides a structured, powerful, and self-documenting way to build complex command-line interfaces with minimal boilerplate, making it a robust foundation for your custom workflows.



## Example

```python
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

```

