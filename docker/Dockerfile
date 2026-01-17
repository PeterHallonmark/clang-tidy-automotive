FROM docker.io/silkeh/clang:20

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    clang-20 \
    llvm-20-dev \
    libclang-20-dev \
    clang-tidy-20 \
    cmake \
    ninja-build \
    git \
    libedit-dev \
    zlib1g-dev \
    libzstd-dev \
    libcurl4-openssl-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
