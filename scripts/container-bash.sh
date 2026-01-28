#!/usr/bin/env bash
# ==============================================================================
# container-bash.sh
#
# Starts an interactive containerized development environment.
# Mounts the current project into /workspace.
# ==============================================================================

set -euo pipefail

IMAGE_NAME="clang-tidy-automotive"
DOCKERFILE_DIR="docker"
WORKDIR="/workspace"

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

# Ensure image exists
# ------------------------------------------------------------------------------
if ! ${RUNTIME} image inspect "${IMAGE_NAME}" >/dev/null 2>&1; then
  echo "Image '${IMAGE_NAME}' not found. Building..."
  ${RUNTIME} build \
    -t "${IMAGE_NAME}" \
    "${DOCKERFILE_DIR}"
fi

# Run container
# ------------------------------------------------------------------------------
echo "Starting container: ${IMAGE_NAME}"

${RUNTIME} run --rm -it \
  -v "$(pwd):${WORKDIR}" \
  -w "${WORKDIR}" \
  "${IMAGE_NAME}" \
  bash
