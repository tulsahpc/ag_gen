#!/bin/bash

OS=$(uname | tr 'A-Z' 'a-z')

if [[ $OS = 'darwin' ]]; then
    if [[ ! `type "brew"` > /dev/null ]]; then
        echo "Please install homebrew first."
    else
        xcode-select --install > /dev/null
        brew cask install postgres
        brew install cmake redis hiredis cppcheck doxygen valgrind graphviz
    fi
elif [[ $OS = 'linux' ]]; then
    type pacman &> /dev/null
    if [[ $? ]]; then
        sudo pacman -S cmake postgresql redis hiredis cppcheck clang doxygen graphviz
    fi

    type apt-get &> /dev/null
    if [[ $? ]]; then
        sudo apt-get -y install postgresql postgresql-contrib libpq-dev \
            redis-server libhiredis-dev cppcheck clang valgrind \
            doxygen graphviz cmake build-essential
    fi
else
    echo "Your operating system is not currently supported."
fi
