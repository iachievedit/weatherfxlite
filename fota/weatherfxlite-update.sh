#!/bin/bash
#
# weatherfxlite FOTA updater
# Checks GitHub releases for a newer version, builds on-device, and
# swaps the binary only if the build succeeds. Fails silently on any error.
#

set -euo pipefail

REPO="iachievedit/weatherfxlite"
INSTALL_DIR="/usr/local/bin"
BINARY_NAME="weatherfxLite"
SERVICE_NAME="weatherfxlite"
LOGFILE="/var/log/weatherfxlite-update.log"

# Source directory where the repo is cloned
SOURCE_DIR="/home/$(whoami)/weatherfxlite"

log() {
  echo "$(date '+%Y-%m-%d %H:%M:%S') $*" >> "$LOGFILE" 2>/dev/null || true
}

cleanup() {
  if [ -n "${WORKTREE_DIR:-}" ] && [ -d "${WORKTREE_DIR:-}" ]; then
    cd "$SOURCE_DIR"
    git worktree remove --force "$WORKTREE_DIR" 2>/dev/null || true
  fi
}
trap cleanup EXIT

# Ensure log file is writable
touch "$LOGFILE" 2>/dev/null || LOGFILE="/tmp/weatherfxlite-update.log"

log "=== FOTA update check started ==="

# Validate source directory
if [ ! -d "$SOURCE_DIR/.git" ]; then
  log "ERROR: Source directory $SOURCE_DIR is not a git repo"
  exit 0  # Silent failure
fi

cd "$SOURCE_DIR"

# Fetch current installed version
if [ ! -f "$SOURCE_DIR/VERSION" ]; then
  log "ERROR: VERSION file not found"
  exit 0
fi
CURRENT_VERSION=$(tr -d '[:space:]' < "$SOURCE_DIR/VERSION")
log "Current version: $CURRENT_VERSION"

# Query GitHub for latest release tag
LATEST_TAG=$(curl -sf \
  "https://api.github.com/repos/${REPO}/releases/latest" \
  | grep '"tag_name"' \
  | head -1 \
  | sed 's/.*"tag_name": *"//;s/".*//')

if [ -z "$LATEST_TAG" ]; then
  log "ERROR: Could not fetch latest release from GitHub"
  exit 0
fi

# Strip leading 'v' if present for comparison
LATEST_VERSION="${LATEST_TAG#v}"
log "Latest release: $LATEST_VERSION (tag: $LATEST_TAG)"

# Compare versions — exit if already up to date
if [ "$CURRENT_VERSION" = "$LATEST_VERSION" ]; then
  log "Already up to date"
  exit 0
fi

log "New version available: $LATEST_VERSION (current: $CURRENT_VERSION)"

# Fetch the new tag
git fetch --tags origin 2>> "$LOGFILE" || { log "ERROR: git fetch failed"; exit 0; }

# Create a temporary worktree to build in (don't touch the main checkout)
WORKTREE_DIR=$(mktemp -d /tmp/weatherfxlite-build-XXXXXX)
git worktree add "$WORKTREE_DIR" "$LATEST_TAG" 2>> "$LOGFILE" || {
  log "ERROR: git worktree add failed for tag $LATEST_TAG"
  exit 0
}

log "Building in worktree: $WORKTREE_DIR"
cd "$WORKTREE_DIR"

# Build — if this fails, we bail and keep the current version
if ! ./build.sh >> "$LOGFILE" 2>&1; then
  log "ERROR: Build failed for version $LATEST_VERSION — keeping current version"
  exit 0
fi

# Verify the binary was actually produced
if [ ! -f "build/$BINARY_NAME" ]; then
  log "ERROR: Build succeeded but binary not found"
  exit 0
fi

log "Build succeeded, installing new binary"

# Install the new binary
sudo cp "build/$BINARY_NAME" "$INSTALL_DIR/$BINARY_NAME" 2>> "$LOGFILE" || {
  log "ERROR: Failed to copy binary to $INSTALL_DIR"
  exit 0
}

# Update the VERSION file in the source directory
echo "$LATEST_VERSION" > "$SOURCE_DIR/VERSION"

# Restart the service
sudo systemctl restart "$SERVICE_NAME" 2>> "$LOGFILE" || {
  log "WARNING: Failed to restart $SERVICE_NAME service"
  exit 0
}

log "Successfully updated to version $LATEST_VERSION and restarted service"
