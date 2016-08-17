#!/bin/bash

function init() {
    echo "${OPTIONS[0]}"
    if [[ ! -z "${OPTIONS[0]}" ]]; then
        DB=${OPTIONS[0]}
    else
        DB=ag_gen
    fi

    if [[ ! -z "${OPTIONS[1]}" ]]; then
        DB_SQL=${OPTIONS[1]}
    else
        DB_SQL=example_data
    fi

    echo "Database: $DB"
    echo "SQL File: $DB_SQL"
    echo ""

    # Check if ag_gen exists
    psql -lqt | cut -d \| -f 1 | grep -qw $DB
    if [ $? -eq 0 ]; then
        OUTPUT="$(dropdb $DB 2>&1 > /dev/null)"
        if [[ $? != 0 ]]; then
            echo "Error dropping database $DB"
            echo "Output:"
            echo "$OUTPUT"
            exit 1
        fi
        echo "Dropped old $DB"
    fi

    createdb $DB

    echo "Created new $DB"

    psql $DB < sql/schema.sql 2>&1 > /dev/null
    psql $DB < sql/$DB_SQL.sql 2>&1 > /dev/null

    echo "Database Populated"
}

function usage() {
    echo "Usage: ./db_manage.sh ACTION [OPTIONS]"
    echo ""
    echo "  Actions:"
    echo "      init - initialize database"
}

if [[ -z "$1" ]]; then
    usage
    exit 1
fi

ACTION=$1
OPTIONS=(${@:2})

case $ACTION in
    "init")
        init
        ;;
    *)
        usage
        ;;
esac
