#!/bin/bash

function tester {
    printf "\n***** $1 Test\n"
    printf "~~~ Output:\n"

    valgrind -q --tool=memcheck --error-exitcode=1 --leak-check=yes --show-leak-kinds=definite --errors-for-leak-kinds=definite $@
    RESULTS+=($?)
}

function printer {
    # echo $2
    if [[ "$2" = "1" ]]; then
        succ="Failure"
    else
        succ="Success"
    fi
    printf "\n***** %s\n~~~ Valgrind Results: %s\n" "$1" $succ
}

TESTS=("bin/ag_gen -n home")
RESULTS=()

if [[ "$1" = "all" ]]; then
    TESTS+=(bin/*_test)
fi

for test in "${TESTS[@]}"; do
    tester $test
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
