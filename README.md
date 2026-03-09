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

TBD documentation needs to be updated since it uses docker and cmake now.

### Prerequisites

Clang 20 (or compatible version)

### Setup Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/PeterHallonmark/clang-tidy-automotive.git
   cd clang-tidy-automotive
   ```

2. TBD: documentation needs to be updated since it uses docker and cmake now.

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

## Contributing

Feel free to open issues or pull requests if you'd like to contribute to
this project. Contributions are always welcome!

**Note:** This project is not officially associated with MISRA or LLVM.
Any contributions or code changes are not officially endorsed by MISRA or 
LLVM.
