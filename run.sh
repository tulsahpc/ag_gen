#!/usr/bin/env bash

./db_manage.sh -d ag_gen2 -f
./build/ag_gen -g ag.dot -n $1 -x $2
#dot -Tsvg -o ag.svg ag.dot

