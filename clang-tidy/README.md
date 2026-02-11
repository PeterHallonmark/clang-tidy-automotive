## clang-tidy-standalone

This is an empty `clang-tidy` standalone version, you can build it without building 
the whole LLVM(which takes a long). Current version is based on llvm-20 so you have 
to use corresponding packages. A little bit more of a motivation is in
this [article](https://oleksandrkvl.github.io/2020/10/06/building-clang-tidy-without-llvm.html).

## About this fork

This repository is a fork of:
https://github.com/OleksandrKvl/clang-tidy-standalone

The original project provided a minimal proof-of-concept for building
clang-tidy outside the LLVM monorepo.

This fork significantly restructures the project to serve as:
- a stable clang-tidy host ("golden source")
- a foundation for building external clang-tidy plugins
- a Docker-based, reproducible build environment
- multi-LLVM-version support

As a result, the directory structure and build system differ substantially
from the original project.

### Usage

Install all LLVM 20 [packages](https://apt.llvm.org/).    
Follow the [official manual](https://clang.llvm.org/extra/clang-tidy/Contributing.html).    
Build as usual:

```
mkdir build
cd build
cmake ..
cmake --build .
```
