# ag_gen: Attack Graph Generation System

## Installation

- Build system: clang/gcc, make/cmake
- Dependencies: PostgreSQL, Redis
- Tools: editorconfig, cppcheck

### Ubuntu

    sudo apt update && sudo apt install -y build-essentials \
        postgresql postgresql-server postgresql-contrib libpq5 \
        libpq-dev redis-server redis-tools libhiredis0.13 libhiredis-dev

### Mac OSX

    xcode-select --install # only if Xcode is not already installed
    brew install postgresql redis

## Coding Style

- Use tabs with a width 8.
- Functions have bracket on new line.
- All other uses of bracket are on the same line.
- Constants are all UPPERCASE.
- Variables are snake case, NOT camelcase.
- Variables are declared at the top of the block it is in.
- The star in a pointer is always aligned with the variable/function name.
- NEVER typedef structs, unions, or enums.

### Example

    #define CONSTANT 500

    int main(int argc, char *argv[])
    {
        ...
    }

    for(int i=0; i<5; i++) {
        ...
    }

    int tmp;
    char *my_string;

