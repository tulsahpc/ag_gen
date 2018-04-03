# ag_gen: Attack Graph Generation System

## Installation

### Dependencies

Run the `deps.sh` command to install necessary dependencies.
This script supports debian based systems and Mac OSX (with homebrew)

### PostgreSQL

You must have permission to be able to access Postgres from the C++ application.
See: https://www.postgresql.org/docs/9.5/static/auth-pg-hba-conf.html

#### Populate the database

Use the `db_manage.sh` utility to populate the database (this will overwrite anything in the `ag_gen` database).
An example use of this is:

    ./db_manage.sh -s sql/schema.sql -i sql/test_data.sql -f

## Building

This application uses CMake to build.

    mkdir build
    cd build
    cmake ../

Build the application:

    make

## Configuration

Copy the `config_default.txt` to `config.txt`.

- server: url to postgresql server
- db: name of database to use
- login: username to login with
- password (optional): password to log into database with

## Running

    ./ag_gen -n home

This doesn't really do anything cool yet.
`./ag_gen -h` for the help menu.

## Tests

Tests don't work currently.

## Documentation

Doxygen doesn't work either.

## Contributing

### Editorconfig

When contributing code, please install the "editorconfig" plugin for your text editor.

- Adds extra newline to end of file if not already there.
- Removes whitespace at of lines
- Automatically sets indentation to tabs set to 4 spaces

### TODO

- Pluggable architecture for different databases
