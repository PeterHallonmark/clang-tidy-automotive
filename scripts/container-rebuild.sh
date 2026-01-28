#!/usr/bin/env bash
# ==============================================================================
# rebuild-container.sh
#
# Rebuilds the clang-tidy automotive container image.
# Forces a clean rebuild (no cache).
# ==============================================================================

set -euo pipefail

IMAGE_NAME="clang-tidy-automotive"
DOCKERFILE_DIR="docker"

# Detects container runtime (podman or docker)
# ------------------------------------------------------------------------------
if command -v podman >/dev/null 2>&1; then
  RUNTIME="podman"
elif command -v docker >/dev/null 2>&1; then
  RUNTIME="docker"
else
  echo "Error: neither podman nor docker found in PATH" >&2
  exit 1
fi

echo "Using container engine: ${RUNTIME}"

# Rebuilds container image
# ------------------------------------------------------------------------------
echo "Rebuilding image: ${IMAGE_NAME}"

${RUNTIME} build \
  --no-cache \
  -t "${IMAGE_NAME}" \
  "${DOCKERFILE_DIR}"

echo "Done."
