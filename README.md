# ag_gen: Attack Graph Generation System

## Installation

### Dependencies

Run the `deps.sh` command to install necessary dependencies.
This script supports debian based systems and Mac OSX (with homebrew)

### PostgreSQL

#### Create a user

Use the createuser and createdb scripts to create the necessary account and database in PostgreSQL.

    createuser -d -l -P ag_gen
    createdb -O ag_gen ag_gen

#### Populate the database

Use the `db_manage.sh` utility to populate the database (this will overwrite anything in the `ag_gen` database).
An example use of this is:

    ./db_manage.sh -d ag_gen

## Building

This application uses CMake to build.

    mkdir build
    cd build

For debug builds:

    cmake -DCMAKE_BUILD_TYPE=Debug ../

For release builds:

    cmake -DCMAKE_BUILD_TYPE=Release ../

Build the application:

    make ag_gen

## Configuration

RAGE uses an ini-style configuration, located in config.ini.

- name: name of the database
- host: IP or hostname of the database server
- port: port number of the database server
- username: database user name
- password (optional): database password

## Running

Execute example from the examples directory.

    ./ag_gen -n ../examples/thesis_example.nm -x ../examples/thesis_example.xp

## Contributing

### Editorconfig

When contributing code, please install the "editorconfig" plugin for your text editor.

- Adds extra newline to end of file if not already there.
- Removes whitespace at end of lines
- Automatically sets indentation to tabs set to 4 spaces
