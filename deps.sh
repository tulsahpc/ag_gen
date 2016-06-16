#!/bin/bash

OS=$(uname | tr 'A-Z' 'a-z')

if [[ $OS = 'darwin' ]]; then
    if [[ ! `type "brew"` > /dev/null ]]; then
        echo "Please install homebrew first."
    else
        brew install postgresql redis hiredis cppcheck doxygen cmake valgrind graphviz
    fi
elif [[ $OS = 'linux' ]]; then
    sudo apt-get -y install postgresql postgresql-contrib libpq-dev \
        redis-server libhiredis-dev cppcheck clang valgrind \
        doxygen cmake graphviz
else
    echo "Your operating system is not currently not supported."
fi
