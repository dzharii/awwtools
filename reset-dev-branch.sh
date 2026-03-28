#!/usr/bin/env bash

# reset-dev-branch.sh
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
#   ./reset-dev-branch.sh
#   ./reset-dev-branch.sh origin
#
# Notes:
#   - The current branch is treated as your dev branch.
#   - The default remote is "origin".
#   - If your dev branch is protected against force-push, the push will fail.
#   - This script intentionally rewrites history on the current branch.

set -Eeuo pipefail

REMOTE="${1:-origin}"

timestamp() {
  date +"%Y-%m-%d %H:%M:%S"
}

log() {
  printf '[%s] %s\n' "$(timestamp)" "$*"
}

die() {
  printf '[%s] ERROR: %s\n' "$(timestamp)" "$*" >&2
  exit 1
}

run() {
  log "Running: $*"
  "$@"
}

cleanup_on_error() {
  local exit_code=$?
  printf '[%s] ERROR: Script failed with exit code %s\n' "$(timestamp)" "$exit_code" >&2
  exit "$exit_code"
}

trap cleanup_on_error ERR

log "Starting branch reset workflow"
log "Remote: $REMOTE"

run git rev-parse --is-inside-work-tree >/dev/null 2>&1 || die "Not inside a Git repository."

CURRENT_BRANCH="$(git symbolic-ref --quiet --short HEAD 2>/dev/null || true)"
if [[ -z "$CURRENT_BRANCH" ]]; then
  die "Detached HEAD detected. Check out your dev branch first."
fi

log "Current branch detected: $CURRENT_BRANCH"

# Ensure remote exists.
run git remote get-url "$REMOTE" >/dev/null 2>&1 || die "Remote '$REMOTE' does not exist."

# Fetch first so origin/HEAD and refs are current.
log "Fetching latest refs from remote and pruning deleted branches"
run git fetch --prune "$REMOTE"

# Determine remote default branch.
REMOTE_HEAD_REF="$(git symbolic-ref --quiet --short "refs/remotes/${REMOTE}/HEAD" 2>/dev/null || true)"
if [[ -n "$REMOTE_HEAD_REF" ]]; then
  DEFAULT_BRANCH="${REMOTE_HEAD_REF#${REMOTE}/}"
else
  log "Could not detect ${REMOTE}/HEAD automatically. Falling back to 'main' if it exists, otherwise 'master'."
  if git show-ref --verify --quiet "refs/remotes/${REMOTE}/main"; then
    DEFAULT_BRANCH="main"
  elif git show-ref --verify --quiet "refs/remotes/${REMOTE}/master"; then
    DEFAULT_BRANCH="master"
  else
    die "Could not determine the remote default branch."
  fi
fi

log "Remote default branch detected: $DEFAULT_BRANCH"

if [[ "$CURRENT_BRANCH" == "$DEFAULT_BRANCH" ]]; then
  die "You are currently on '$DEFAULT_BRANCH'. This script must be run from your dev branch, not the main/default branch."
fi

# Require a clean working tree, including untracked files.
if ! git diff --quiet || ! git diff --cached --quiet || [[ -n "$(git ls-files --others --exclude-standard)" ]]; then
  die "Working tree is not clean. Commit, stash, or remove your changes first, including untracked files."
fi

# Ensure the remote default branch exists.
run git show-ref --verify --quiet "refs/remotes/${REMOTE}/${DEFAULT_BRANCH}" || \
  die "Remote branch '${REMOTE}/${DEFAULT_BRANCH}' was not found."

# Ensure current local branch exists locally.
run git show-ref --verify --quiet "refs/heads/${CURRENT_BRANCH}" || \
  die "Local branch '${CURRENT_BRANCH}' was not found."

BACKUP_BRANCH="backup/${CURRENT_BRANCH}-before-reset-$(date +%Y%m%d-%H%M%S)"

log "Creating safety backup branch: $BACKUP_BRANCH"
run git branch "$BACKUP_BRANCH" "$CURRENT_BRANCH"

log "Resetting '${CURRENT_BRANCH}' to '${REMOTE}/${DEFAULT_BRANCH}'"
run git reset --hard "${REMOTE}/${DEFAULT_BRANCH}"

log "Force-pushing rewritten branch to remote with lease protection"
run git push --force-with-lease "$REMOTE" "HEAD:${CURRENT_BRANCH}"

log "Done."
log "Your dev branch '${CURRENT_BRANCH}' now matches '${REMOTE}/${DEFAULT_BRANCH}'."
log "Backup branch created locally: ${BACKUP_BRANCH}"
log "You can now continue working on '${CURRENT_BRANCH}' from a clean state."