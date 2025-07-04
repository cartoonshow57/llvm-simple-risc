FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Update the apt index and install required packages
RUN apt-get update && \
    apt-get install -y \
      build-essential \        # GCC, make, and core build tools
      cmake \                  # Project configuration
      ninja-build \            # Fast build system
      llvm \                   # LLVM core tools (opt, llc, etc.)
      clang \                  # Clang C/C++ frontend
      lld \                    # LLVM linker
    && rm -rf /var/lib/apt/lists/*
