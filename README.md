# Clang-Tidy Automotive

This repository provides a collection of Clang-Tidy checks inspired by the 
goals and safety principles behind coding standards such as MISRA C:2023 
and MISRA C:2012.

The project does **not** reproduce any MISRA copyright content, including 
rule names, rule texts, or associated documentation. All the MISRA mapping
is put into a separate repository, see
[Clang-Tidy MISRA](https://github.com/PeterHallonmark/clang-tidy-misra)
for more information.

## Getting Started

To build clang-tidy Automotive, follow these steps:

### Prerequisites 

Clang 20 (or compatible version)

### Setup Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/PeterHallonmark/clang-tidy-automotive.git
   cd clang-tidy-automotive
   ```

2. Run the following setup scripts to build Clang-Tidy Automotive:
   ```bash
   ./download.sh
   ./setup.sh
   ./configure.sh
   ./build.sh
   ```

**Note:** The scripts will need to be adjusted to use a Docker container in
the future. This is part of the TODO list.

## Usage

To use the Automotive support in clang-tidy, enable the rules automotive-* 
when using clang-tidy. The example below assumes the avoid-goto.c test file 
and the root directory of clang-tidy-automotive.
   ```bash
   ./build/bin/clang-tidy --checks="automotive-avoid-goto" test/checkers/automotive/statement/avoid-goto.c -- 
   ```

See [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) for more information.

## License

The repository follows LLVM's license structure to make it easier to upstream
changes to the LLVM project if and when that happens. :)

## Design note: diagnostic mapping layer

A significant part of this project right now is the diagnostic mapping layer.

The purpose of the mapping layer is to strictly separate rule content from 
the clang-tidy checks themselves. This separation is essential in order to:

- avoid embedding copyrighted MISRA rule text in the codebase
- allow different interpretations or future revisions of MISRA rules without
  rewriting the checks
- Map native clang diagnostics towards MISRA rules
- make the design acceptable for potential upstreaming in LLVM

The mapping layer is still under active development. Once it is complete and
proven, an RFC will be written and proposed to LLVM to evaluate whether this
architecture could be upstreamed.

For background and context, see the
[LLVM discussion](https://discourse.llvm.org/t/will-clang-frontend-plan-accept-misra-check-tools/84754/33?u=peterhallonmark).

## Future Improvements

Move setup scripts to a Docker container for easier setup and environment
management.

## Contributing

Feel free to open issues or pull requests if you'd like to contribute to
this project. Contributions are always welcome!

**Note:** This project is not officially associated with MISRA or LLVM.
Any contributions or code changes are not officially endorsed by MISRA or 
LLVM.
