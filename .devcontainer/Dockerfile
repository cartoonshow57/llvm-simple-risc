FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

RUN apt-get update && \
    apt-get install -y \
      cmake \
      ninja-build \
      llvm \
      clang \
      lld \
    && rm -rf /var/lib/apt/lists/*
