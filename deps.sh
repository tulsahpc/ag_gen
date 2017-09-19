#!/bin/bash

OS=$(uname | tr 'A-Z' 'a-z')

if [[ $OS = 'darwin' ]]; then
    if [[ ! `type "brew"` > /dev/null ]]; then
        echo "Please install homebrew first."
    else
        xcode-select --install > /dev/null
        brew cask install postgres
        brew install postgresql cmake boost redis hiredis cppcheck doxygen valgrind graphviz
    fi
elif [[ $OS = 'linux' ]]; then
    type pacman &> /dev/null
    if [[ $? ]]; then
        sudo pacman -S cmake postgresql boost redis hiredis cppcheck clang doxygen graphviz
    fi

    type apt-get &> /dev/null
    if [[ $? ]]; then
        sudo apt-get -y install libboost-graph-dev postgresql postgresql-contrib libpq-dev \
            redis-server libhiredis-dev cppcheck clang valgrind \
            doxygen graphviz cmake build-essential bison flex \
            libssl-dev
    fi
else
    echo "Your operating system is not currently supported."
fi
