#!/bin/bash

function init() {
    echo "${OPTIONS[0]}"
    if [[ ! -z "${OPTIONS[0]}" ]]; then
        DB=${OPTIONS[0]}
    else
        echo "Please supply a database with -d"
        exit 1
    fi

    if [[ ! -z "${OPTIONS[1]}" ]]; then
        DB_SQL=${OPTIONS[1]}
    else
        echo "Please supply a data file with -f"
        exit 1
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
    psql $DB < $DB_SQL 2>&1 > /dev/null

    echo "Database Populated"
}

function usage() {
    echo "Usage: ./db_manage.sh ACTION DATABASE FILE"
    echo ""
    echo "  Actions:"
    echo "      init - initialize database"
    echo ""
    echo "  Options:"
    echo "       database - which db to initialize"
    echo "       file - the data to initialize the db with"
    echo ""
}

while getopts "a:d:f:" opt; do
    case $opt in
        a)
            echo "Action: $OPTARG"
            ;;
        d)
            echo "Database: $OPTARG"
            ;;
        f)
            echo "File: $OPTARG"
            ;;
        :)
            echo "Option required for $OPTARG - $OPTERR"
            ;;
        *)
            usage
            ;;
    esac
done



















