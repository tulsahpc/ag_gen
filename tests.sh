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
fi

echo "Output: "
echo $OUTPUT

# tester bin/db_test "Database"
# tester bin/redis_test "Redis"
# tester bin/string_test "String"
# tester bin/perm_test "Permutation"
# tester bin/list_test "List"
# tester bin/hash_test "Hash"
