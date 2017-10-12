#!/bin/bash

OS=$(uname | tr 'A-Z' 'a-z')

if [[ $OS = 'darwin' ]]; then
    if [[ ! `type "brew"` > /dev/null ]]; then
        echo "Please install homebrew first."
    else
        xcode-select --install > /dev/null
        brew cask install postgres
        brew install postgresql cmake boost cppcheck doxygen graphviz
    fi
elif [[ $OS = 'linux' ]]; then
    if [[ `type "pacman"` > /dev/null ]]; then
        sudo pacman -S cmake postgresql boost cppcheck clang doxygen graphviz
    elif [[ `type "apt-get"` > /dev/null ]]; then
        sudo apt-get -y install libboost-graph-dev postgresql postgresql-contrib \
            libpq-dev cppcheck clang valgrind doxygen graphviz cmake \
            build-essential bison flex libssl-dev
    else
        echo "Your distro is currently not supported."
    fi
else
    echo "Your operating system is not currently supported."
fi
