FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Update the apt index and install required packages
RUN apt-get update && \
    apt-get install -y \
      build-essential \
      cmake \
      ninja-build \
      llvm \
      clang \
      lld \
    && rm -rf /var/lib/apt/lists/*
