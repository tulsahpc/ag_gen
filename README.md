# ag_gen: Attack Graph Generation System

## Installation

Build system: clang/gcc, make/cmake  
Dependencies: PostgreSQL, Redis  
Tools: editorconfig, cppcheck

### Dependencies

Run the `deps.sh` command to install necessary dependencies. This script supports debian based systems and Mac OSX (with homebrew)

### PostgreSQL

You must have permission to be able to access Postgres from the C application. Currently, there is no support for passwords, so you must enable full access to local users. See: https://www.postgresql.org/docs/9.5/static/auth-pg-hba-conf.html

## Building

Production build:

    make

Debug build:

    make debug

## Running

    ./ag_gen -n home

This doesn't really do anything cool yet. ./ag_gen -h for the help menu.

## Tests

Execute tests with

    make test

If you want debugging enabled for the tests, run

    make debug

before `make test`

## Documentation

The documentation for the source code can be build with

    make docs

## Contributing

### Editorconfig

When contributing code, please install the "editorconfig" plugin for your text editor.

- Adds extra newline to end of file if not already there.
- Removes whitespace at of lines
- Automatically sets indentation to tabs set to 8 spaces

### Coding Style

- Use tabs with a width 8.
- Functions have bracket on new line.
- All other uses of bracket are on the same line.
- Constants are all UPPERCASE.
- Variables are snake_case.
- Structs, enums, and unions are CamelCase.
- Variables are declared at the top of the block it is in.
- NEVER typedef structs, unions, or enums.
- The star in a pointer is always aligned with the variable/function name.

#### Example

```C
#define CONSTANT 500

struct ExampleStruct {
    int id;
    char *string;
}

int main(int argc, char *argv[])
{                                           // brackets on new line for functinons
        int snake_case;                     // 8 spaces for a tab
        char *string;                       // pointer star aligned next to variable name
        struct ExampleStruct my_example;
}

for(int i=0; i<5; i++) {                    // brackets on same line for everything else
        ...
}
```

