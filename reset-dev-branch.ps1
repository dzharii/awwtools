# reset-dev-branch.ps1
#
# Purpose:
#   After your PR has been squash-merged into the remote default branch
#   (usually main), this script rewrites your current dev branch so that:
#
#     current-dev-branch == origin/<default-branch>
#
#   This drops the previous dev-branch-only history from the branch tip,
#   keeps the same branch name, and lets you start the next cycle cleanly.
#
# What it does:
#   1. Verifies you are inside a Git repository.
#   2. Detects the current branch automatically.
#   3. Refuses to run on the remote default branch itself.
#   4. Refuses to run if the working tree is not clean.
#   5. Fetches and prunes the remote.
#   6. Detects the remote default branch, usually main.
#   7. Creates a local backup branch before rewriting anything.
#   8. Hard-resets the current branch to origin/<default-branch>.
#   9. Force-pushes with --force-with-lease.
#
# Usage:
#   .\reset-dev-branch.ps1
#   .\reset-dev-branch.ps1 -Remote origin
#
# Notes:
#   - The current branch is treated as your dev branch.
#   - The default remote is "origin".
#   - If your dev branch is protected against force-push, the push will fail.
#   - This script intentionally rewrites history on the current branch.

param(
    [string]$Remote = "origin"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-Timestamp {
    Get-Date -Format "yyyy-MM-dd HH:mm:ss"
}

function Log {
    param([string]$Message)
    Write-Host "[$(Get-Timestamp)] $Message"
}

function Fail {
    param([string]$Message)
    throw "[$(Get-Timestamp)] ERROR: $Message"
}

function Invoke-Git {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$Args
    )

    Log ("Running: git " + ($Args -join " "))
    & git @Args
    if ($LASTEXITCODE -ne 0) {
        Fail ("Git command failed: git " + ($Args -join " "))
    }
}

try {
    Log "Starting branch reset workflow"
    Log "Remote: $Remote"

    & git rev-parse --is-inside-work-tree *> $null
    if ($LASTEXITCODE -ne 0) {
        Fail "Not inside a Git repository."
    }

    $currentBranch = (& git symbolic-ref --quiet --short HEAD 2>$null)
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($currentBranch)) {
        Fail "Detached HEAD detected. Check out your dev branch first."
    }

    $currentBranch = $currentBranch.Trim()
    Log "Current branch detected: $currentBranch"

    & git remote get-url $Remote *> $null
    if ($LASTEXITCODE -ne 0) {
        Fail "Remote '$Remote' does not exist."
    }

    Log "Fetching latest refs from remote and pruning deleted branches"
    Invoke-Git -Args @("fetch", "--prune", $Remote)

    $remoteHeadRef = (& git symbolic-ref --quiet --short "refs/remotes/$Remote/HEAD" 2>$null)
    $defaultBranch = $null

    if ($LASTEXITCODE -eq 0 -and -not [string]::IsNullOrWhiteSpace($remoteHeadRef)) {
        $remoteHeadRef = $remoteHeadRef.Trim()
        $prefix = "$Remote/"
        if ($remoteHeadRef.StartsWith($prefix)) {
            $defaultBranch = $remoteHeadRef.Substring($prefix.Length)
        }
    }

    if ([string]::IsNullOrWhiteSpace($defaultBranch)) {
        Log "Could not detect $Remote/HEAD automatically. Falling back to 'main' if it exists, otherwise 'master'."

        & git show-ref --verify --quiet "refs/remotes/$Remote/main"
        if ($LASTEXITCODE -eq 0) {
            $defaultBranch = "main"
        }
        else {
            & git show-ref --verify --quiet "refs/remotes/$Remote/master"
            if ($LASTEXITCODE -eq 0) {
                $defaultBranch = "master"
            }
            else {
                Fail "Could not determine the remote default branch."
            }
        }
    }

    Log "Remote default branch detected: $defaultBranch"

    if ($currentBranch -eq $defaultBranch) {
        Fail "You are currently on '$defaultBranch'. This script must be run from your dev branch, not the main/default branch."
    }

    & git diff --quiet
    $worktreeDirty = ($LASTEXITCODE -ne 0)

    & git diff --cached --quiet
    $indexDirty = ($LASTEXITCODE -ne 0)

    $untracked = & git ls-files --others --exclude-standard
    $hasUntracked = -not [string]::IsNullOrWhiteSpace(($untracked | Out-String).Trim())

    if ($worktreeDirty -or $indexDirty -or $hasUntracked) {
        Fail "Working tree is not clean. Commit, stash, or remove your changes first, including untracked files."
    }

    & git show-ref --verify --quiet "refs/remotes/$Remote/$defaultBranch"
    if ($LASTEXITCODE -ne 0) {
        Fail "Remote branch '$Remote/$defaultBranch' was not found."
    }

    & git show-ref --verify --quiet "refs/heads/$currentBranch"
    if ($LASTEXITCODE -ne 0) {
        Fail "Local branch '$currentBranch' was not found."
    }

    $backupBranch = "backup/$currentBranch-before-reset-$(Get-Date -Format 'yyyyMMdd-HHmmss')"

    Log "Creating safety backup branch: $backupBranch"
    Invoke-Git -Args @("branch", $backupBranch, $currentBranch)

    Log "Resetting '$currentBranch' to '$Remote/$defaultBranch'"
    Invoke-Git -Args @("reset", "--hard", "$Remote/$defaultBranch")

    Log "Force-pushing rewritten branch to remote with lease protection"
    Invoke-Git -Args @("push", "--force-with-lease", $Remote, "HEAD:$currentBranch")

    Log "Done."
    Log "Your dev branch '$currentBranch' now matches '$Remote/$defaultBranch'."
    Log "Backup branch created locally: $backupBranch"
    Log "You can now continue working on '$currentBranch' from a clean state."
}
catch {
    Write-Error $_.Exception.Message
    exit 1
}