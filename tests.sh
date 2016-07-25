#!/bin/bash

tester() {
    printf "\n***** $1 Test\n"
    printf "~~~ Output:\n"

    valgrind -q --error-exitcode=1 --tool=memcheck $1
    RESULTS+=($?)
}

printer() {
    if [[ "$2" = "1" ]]; then
        succ="Failure"
    else
        succ="Success"
    fi
    printf "\n***** %s\n~~~ Valgrind Results: %s\n" "$1" $succ
}

TESTS=(bin/*_test)
RESULTS=()

for test in ${TESTS[@]}; do
    tester $test
done

for idx in ${!TESTS[@]}; do
    printer ${TESTS[$idx]} ${RESULTS[$idx]}
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
