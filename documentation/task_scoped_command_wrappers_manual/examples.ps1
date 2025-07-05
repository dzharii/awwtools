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