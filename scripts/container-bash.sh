#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME=clang-tidy-automotive
DOCKERFILE=docker/Dockerfile

# ------------------------------------------------------------
# Detect container runtime
# ------------------------------------------------------------
if command -v podman >/dev/null 2>&1; then
  RUNTIME=podman
  echo "Using podman"
elif command -v docker >/dev/null 2>&1; then
  RUNTIME=docker
  echo "Using docker"
else
  echo "ERROR: Neither podman nor docker found in PATH"
  exit 1
fi

# ------------------------------------------------------------
# Check if image exists
# ------------------------------------------------------------

if ! $RUNTIME image inspect "$IMAGE_NAME" >/dev/null 2>&1; then
  echo "Image '$IMAGE_NAME' not found. Building..."
  $RUNTIME build -t "$IMAGE_NAME" -f "$DOCKERFILE" .
fi

# ------------------------------------------------------------
# Run container
# ------------------------------------------------------------

echo "Starting '$IMAGE_NAME'"
$RUNTIME run --rm -it \
  -v "$(pwd):/workspace" \
  -w /workspace \
  "$IMAGE_NAME" \
  bash
