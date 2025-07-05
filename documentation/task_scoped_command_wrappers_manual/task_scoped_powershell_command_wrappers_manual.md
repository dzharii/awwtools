# Task-Scoped PowerShell Command Wrappers: A Design and Maintenance Manual
2025-06-28

## Introduction

This manual provides a comprehensive guide to designing, creating, and maintaining task-scoped PowerShell command wrappers. These wrappers are small, specialized scripts that tame large, general-purpose command-line interfaces (CLIs) by exposing only the specific functionality you need for your workflows.

Although the examples below often feature `git` and `ffmpeg`, the pattern is universal. It applies to any CLI or API you use, including but not limited to Terraform, Docker, the Azure CLI, `kubectl`, and ImageMagick. Whenever you find yourself repeatedly typing a complex sequence of flags or performing a manual, error-prone task, consider encapsulating that workflow into a script following the principles outlined here.

## 1. Purpose and Philosophy

The core philosophy behind a task-scoped wrapper is to narrow a heavyweight toolchain to the small slice you actually use. Each script should adhere to the following principles:

*   **Define a Single Domain:** Each wrapper should correspond to one logical domain, such as Git operations, video manipulation, or cloud infrastructure management.
*   **Expose Plain-English Commands:** Subcommands should be clear, memorable verb phrases (e.g., `diff-from-main`, `compress-gif`) that describe a specific task.
*   **Provide a Single Entry Point:** The wrapper is the single interface for the task. Callers should never need to remember or use the raw flags of the underlying tool.
*   **Validate and Guide:** The script must validate its inputs, provide clear help text, and print helpful error messages to guide the user.

This approach turns complex tools into predictable, self-documenting building blocks that can be version-controlled, peer-reviewed, and safely integrated into automated pipelines.

## 2. Anatomy of a Wrapper Script

A well-structured wrapper script consists of several distinct parts, each with a specific responsibility.

### 2.1. Parameter Block

Declare all external inputs at the top of the script in a `param` block. This centralizes the script's public interface.

```powershell
param(
    [Parameter(Mandatory = $true)][string]$Command,
    [Parameter()][string]$Name,
    [Parameter()][string]$FilePath,
    [Parameter()][string]$FromTime,
    [Parameter()][string]$ToTime
)
```

**Guidelines:**

*   Use the `[Parameter()]` attribute for all parameters to improve discoverability in editors and command-line environments.
*   Mark only the main `$Command` parameter as mandatory. All other parameters are typically contextual to a specific subcommand.

### 2.2. Command Constants

Define a constant for each subcommand. This practice prevents typos and makes the code easier to maintain, as the command name is defined in exactly one place.

```powershell
$COMMAND_DIFF_FROM_MAIN = "diff-from-main"
$COMMAND_HELP = "help"
```

**Guidelines:**

*   Keep constant names in `kebab-case` for readability and consistency.
*   Sort the constants alphabetically to minimize merge conflicts in version control.

### 2.3. Help Message

The help message is crucial for making the script discoverable and user-friendly. Store it in a PowerShell "here-string" and use string interpolation to reference the command constants, ensuring the help text never drifts out of sync with the implementation.

```powershell
$HELP_MESSAGE = @"
Usage:
   ff.ps1 <command> [options]

Description:
   A wrapper for common FFmpeg video and GIF operations.

Commands:
    $($COMMAND_HELP):
      Shows this help message.

    $($COMMAND_CONVERT_WEBM):
      Converts an input video to the WebM format.
      Example:
         ff.ps1 convert-webm -FilePath "input.mov"
"@
```

**Guidelines:**

*   Wrap the help text at 80 columns for optimal readability in standard terminals.
*   Provide at least one concrete, runnable example for every subcommand.

### 2.4. Utility Functions

Delegate all significant work to private, well-named functions. The main body of the script should only be responsible for dispatching commands.

```powershell
function Invoke-FFmpegConvertWebM {
    param(
        [Parameter(Mandatory = $true)][string]$FilePath
    )

    if (-not (Test-Path -Path $FilePath)) {
        Write-Host "Error: File not found at '$FilePath'" -ForegroundColor Red
        exit 1
    }

    $outputPath = [IO.Path]::ChangeExtension($FilePath, ".webm")
    & ffmpeg -i "$FilePath" -c:v libvpx -c:a libvorbis "$outputPath"
}
```

**Guidelines:**

*   Validate parameters at the beginning of the function and exit with a non-zero status code on failure.
*   Return structured objects (`[pscustomobject]`) whenever possible so that the script's output can be piped, filtered, and tested programmatically.

### 2.5. Dispatcher Switch

The `switch` statement is the script's central router. It takes the user's command, matches it to a constant, and calls the corresponding utility function.

```powershell
switch ($Command.ToLower()) {
    $COMMAND_HELP { Write-Host $HELP_MESSAGE }
    $COMMAND_CONVERT_WEBM { Invoke-FFmpegConvertWebM -FilePath $FilePath }
    Default {
        Write-Host "Unknown command: $Command" -ForegroundColor Red
        Write-Host $HELP_MESSAGE
        exit 1
    }
}
```

**Guidelines:**

*   Convert the input `$Command` to lower-case once to ensure case-insensitive matching.
*   The `Default` block should always handle unknown commands gracefully by printing an error and displaying the help message.
*   Use distinct colors or formatting for error messages to make them stand out.

### 2.6. Post-Execution Footer

A simple timestamp at the end of the script can be invaluable for auditing and debugging.

```powershell
Write-Host "Done: $(Get-Date -Format o)"
```

## 3. Creating a New Wrapper Script

1.  **Choose a Name:** Pick a concise, descriptive file name that identifies the tool or domain (e.g., `aws.ps1`, `docker.ps1`, `image.ps1`).
2.  **Copy Boilerplate:** Clone the file structure from the most mature existing script to ensure consistency.
3.  **Customize:** Remove unrelated constants, functions, and `switch` cases.
4.  **Write Help Text:** Fill in the `$HELP_MESSAGE` template with a clear description and at least one example per command.
5.  **Commit:** Make an initial commit with a passing Pester smoke test to establish a valid baseline.

## 4. Adding a New Subcommand

1.  **Define the Command:** Choose a clear, user-facing verb phrase (e.g., `compress-gif`).
2.  **Add a Constant:** Add a new `$COMMAND_...` constant, maintaining alphabetical order.
3.  **Update Help:** Extend the `$HELP_MESSAGE` with a description of the new command, its parameters, and a working example.
4.  **Implement the Function:** Write a dedicated function that contains the core logic, including parameter validation and output handling.
5.  **Wire the Switch:** Add a new arm to the `switch` statement that calls the function.
6.  **Verify:** Run the script with the `help` command to ensure the new entry renders correctly.
7.  **Test:** Add a Pester test that executes the command and asserts a successful exit code (`0`).
8.  **Commit:** Commit the changes with a descriptive message, e.g., `feat(ff): add compress-gif subcommand`.

## 5. Maintenance Guidelines

*   **Keep it Concise:** Aim to keep each script under 500 lines of code. If a script grows too large, consider splitting it into more specialized wrappers.
*   **Refactor Shared Code:** If you find yourself duplicating helper functions (e.g., for parsing time strings or handling paths) across multiple scripts, refactor them into a common PowerShell module that the wrappers can import.
*   **Log Breaking Changes:** Document any backward-incompatible changes in a `CHANGELOG.md` file.
*   **Automate Quality Checks:** Enforce a pre-commit git hook that runs `Invoke-ScriptAnalyzer` for static analysis and Pester for unit tests.
*   **Conduct Peer Reviews:** Use a checklist for pull requests to ensure that every change meets quality standards: constants added, help updated, mandatory parameters justified, error paths return non-zero, and tests cover both success and failure cases.
*   **Tag Releases:** Use version control tags for releases and pin automation pipelines to specific tags for stable, reproducible builds.

## 6. Version Control and Distribution

*   **Centralize Scripts:** Store all wrapper scripts in a dedicated `scripts/` directory at the root of the repository.
*   **Package as a Module:** Once the collection of wrappers grows (e.g., more than five scripts), consider packaging them as a formal PowerShell module for easier distribution and installation.
*   **Publish Documentation:** Set up a CI/CD pipeline job to automatically generate HTML documentation from the script's help content (using `Get-Help -Full`) and publish it to an internal site for easy browsing.

## 7. Example Workflow: Adding `convert-webm` to `ff.ps1`

This example demonstrates the end-to-end process of adding a new subcommand.

**1. Define the Constant**
Add the new command to the constants block in `ff.ps1`, maintaining alphabetical order.
```powershell
$COMMAND_CONVERT_WEBM = "convert-webm"
```

**2. Extend the Help Message**
Add a new entry to the `$HELP_MESSAGE` here-string.
```
$($COMMAND_CONVERT_WEBM):
  Converts an input video to WebM format using libvpx/libvorbis.
  Example:
     ff.ps1 convert-webm -FilePath "input.mp4"
```

**3. Implement the Function**
Write the function that performs the conversion, including validation.
```powershell
function Invoke-FFmpegConvertWebM {
    param([string]$FilePath)

    if (-not (Test-Path -Path $FilePath)) {
        Write-Host "Error: Input file not found." -ForegroundColor Red
        exit 1
    }

    $out = [IO.Path]::ChangeExtension($FilePath, ".webm")
    & ffmpeg -i "$FilePath" -c:v libvpx -c:a libvorbis "$out"
}
```

**4. Add to the Switch**
Wire the command to the function in the dispatcher.
```powershell
$COMMAND_CONVERT_WEBM { Invoke-FFmpegConvertWebM -FilePath $FilePath }
```

**5. Update Tests and Commit**
Finally, add a Pester test to validate the new functionality and commit the changes.

## 8. Development Checklist

Use this checklist when adding a new command or creating a new wrapper to ensure no steps are missed.

*   [ ] Constant added alphabetically
*   [ ] Help message updated with description and example
*   [ ] Dedicated function created with parameter validation
*   [ ] Switch case wired to the new function
*   [ ] Unit test written for the success case
*   [ ] `Invoke-ScriptAnalyzer` passes without errors
*   [ ] `CHANGELOG.md` entry added if applicable

## Inspiration

The `kebab-case` command naming convention is inspired by the style of interactive commands in Emacs Lisp. As in Emacs, this pattern empowers you to coin your own verbs and bind them to any sequence of actions, effectively turning your personal workflows into first-class, repeatable commands.



## Example 

```powershell
# ANNOTATED EXAMPLE OF A TASK-SCOPED POWERSHELL WRAPPER
#
# This script demonstrates the design pattern described in the manual.
# It serves as a template for creating new, robust command wrappers.

# Section 2.1: Parameter Block
# ============================
# Declare all external inputs here. Using the [Parameter()] attribute makes them
# discoverable by PowerShell's help system and editors. The $Command parameter
# is the only one marked as mandatory, as it determines which action to take.
param(
    [Parameter(Mandatory = $true)]
    [string]$Command,

    [Parameter()]
    [string]$FilePath,

    [Parameter()]
    [string]$Message
)

# Set the error action preference to 'Stop' to ensure that the script
# exits immediately if any command fails. This is a best practice for
# creating predictable and reliable automation scripts.
$ErrorActionPreference = "Stop"

# A helper variable to get the directory where this script is located.
# Useful for finding related files or modules.
$ThisScriptFolderPath = Split-Path -Parent $MyInvocation.MyCommand.Definition

# Section 2.2: Command Constants
# ==============================
# Define constants for each subcommand. This avoids "magic strings" and makes
# the code easier to refactor and maintain. They are sorted alphabetically
# to reduce version control merge conflicts.
$COMMAND_DO_SOMETHING = "do-something"
$COMMAND_HELP = "help"

# Section 2.3: Help Message
# =========================
# A user-friendly help message stored in a "here-string" (@"..."@).
# It interpolates the command constants to ensure the help text always
# matches the implementation.
$HELP_MESSAGE = @"
Usage:
   examples.ps1 <command> [options]
   aww run examples <command> [options]

Description:
   An example script demonstrating the task-scoped wrapper pattern.

Commands:
    $($COMMAND_HELP):
      Shows this help message.

    $($COMMAND_DO_SOMETHING):
      A sample command that demonstrates calling a utility function.
      Example:
         examples.ps1 do-something --FilePath "./my-file.txt" --Message "Hello World"

"@

# Section 2.4: Utility Functions
# ==============================
# All real work should be delegated to private functions. This keeps the main
# dispatcher logic clean and simple. Functions should validate their own
# parameters and handle their specific logic.
function Invoke-Something {
    param(
        [string]$Path,
        [string]$Text
    )

    Write-Host "Simulating action..."
    Write-Host "  - FilePath: $Path"
    Write-Host "  - Message: $Text"
    # In a real script, you would perform file operations, API calls, etc.
    # For example: Add-Content -Path $Path -Value $Text
    Write-Host "Simulation complete." -ForegroundColor Green
}


# Section 2.5: Dispatcher Switch
# ==============================
# The main switch statement acts as the script's router. It converts the
# input command to lower-case for case-insensitive matching and calls the
# appropriate function or displays help.
switch ($Command.ToLower()) {
    
    $COMMAND_HELP {
        Write-Host $HELP_MESSAGE
    }

    $COMMAND_DO_SOMETHING {
        # Pass the script's parameters on to the utility function.
        Invoke-Something -Path $FilePath -Text $Message
    }

    # The Default block handles any unknown commands, prints an error,
    # shows the help message, and exits with a non-zero status code
    # to signal failure to calling processes.
    Default {
        Write-Host "Error: Unknown command '$Command'" -ForegroundColor Red
        Write-Host ""
        Write-Host $HELP_MESSAGE
        exit 1
    }
}

# Section 2.6: Post-Execution Footer
# ==================================
# A timestamp at the end of the script provides a clear signal that the
# script completed and can be useful for logging and auditing.
Write-Host "Done: $(Get-Date -Format o)"
```

