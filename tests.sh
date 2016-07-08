#!/bin/bash

printf "***** Database Tests\n"
./db_test

printf "\n***** Redis Tests\n"
./redis_test

printf "\n***** String Library Tests\n"
./string_test

printf "\n***** Permutation Tests\n"
./perm_test

printf "\n***** Linked List Tests\n"
./list_test

printf "\n***** Hash Table Tests\n"
./hash_test
