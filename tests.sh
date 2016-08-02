#!/bin/bash

function tester {
    printf "\n***** Test: '$1'\n"
    printf "~~~ Output:\n"

    valgrind -q --tool=memcheck --error-exitcode=1 --leak-check=yes --show-leak-kinds=definite --errors-for-leak-kinds=definite $@
    ERR=$?
    RESULTS+=($ERR)
}

function printer {
    if [[ "$2" = "0" ]]; then
        succ="Success"
    else
        succ="Failure"
    fi
    printf "\n***** Result: '%s'\n~~~ Valgrind: %s\n" "$1" $succ
}

TESTS=("bin/ag_gen -n home")
RESULTS=()

if [[ "$1" = "all" ]]; then
    TESTS+=(bin/test_*)
fi

for test in "${TESTS[@]}"; do
    tester "$test"
done

for idx in ${!TESTS[@]}; do
    printer "${TESTS[$idx]}" "${RESULTS[$idx]}"
done

printf "\n***** Static Analysis: "
OUTPUT=`cppcheck -q --error-exitcode=1 **/*.{c,h} 2>&1`
if [[ "$?" = "0" ]]; then
    printf "No Errors\n"
else
    printf "Errors Found\n"
    echo "~~~ Output: "
    echo $OUTPUT
fi
