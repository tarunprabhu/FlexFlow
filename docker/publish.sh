#! /usr/bin/env bash
set -euo pipefail

# Cd into directory holding this script
cd "${BASH_SOURCE[0]%/*}"

image=${1:-"flexflow-cuda"}
# Check publish specinfer environment image
if [[ "${image}" == @(specinfer-environment-cuda|specinfer-environment-hip_cuda|specinfer-environment-hip_rocm|specinfer-environment-intel) ]]; then
  echo "specinfer does not publish environment images"
  exit 1
fi

# Check valid image name
if [[ "${image}" != @(flexflow-environment-cuda|flexflow-environment-hip_cuda|flexflow-environment-hip_rocm|flexflow-environment-intel|flexflow-cuda|flexflow-hip_cuda|flexflow-hip_rocm|flexflow-intel|specinfer-cuda|specinfer-hip_cuda|specinfer-hip_rocm|specinfer-intel) ]]; then
  echo "Error, image name ${image} is invalid. Choose between 'flexflow-environment-{cuda,hip_cuda,hip_rocm,intel}' and 'flexflow-{cuda,hip_cuda,hip_rocm,intel}'."
  exit 1
fi

# Check that image exists
if [[ "${image}" == @(specinfer-cuda|specinfer-hip_cuda|specinfer-hip_rocm|specinfer-intel) ]]; then 
  SUBSTR="${image:10}"
  docker image inspect "flexflow-${SUBSTR}":latest > /dev/null
else
  docker image inspect "${image}":latest > /dev/null
fi

# Log into container registry
FLEXFLOW_CONTAINER_TOKEN=${FLEXFLOW_CONTAINER_TOKEN:-}
if [ -z "$FLEXFLOW_CONTAINER_TOKEN" ]; then echo "FLEXFLOW_CONTAINER_TOKEN secret is not available, cannot publish the docker image to ghrc.io"; exit; fi
echo "$FLEXFLOW_CONTAINER_TOKEN" | docker login ghcr.io -u flexflow --password-stdin

# Tag image to be uploaded
git_sha=${GITHUB_SHA:-$(git rev-parse HEAD)}
if [ -z "$git_sha" ]; then echo "Commit hash cannot be detected, cannot publish the docker image to ghrc.io"; exit; fi

# If in "inference" branch, which tries to publish "specinfer" images,
# tags the all images as "specinfer-{cuda, hip_cuda, hip_rocm, intel}"; if in others, do as orginal
if [[ "${image}" == @(specinfer-cuda|specinfer-hip_cuda|specinfer-hip_rocm|specinfer-intel) ]]; then 
  SUBSTR="${image:10}"
  docker tag flexflow-"$SUBSTR":latest ghcr.io/flexflow/specinfer-"$SUBSTR":latest
else
  docker tag "$image":latest ghcr.io/flexflow/"$image":latest
fi

# Upload image
docker push ghcr.io/flexflow/"$image":latest