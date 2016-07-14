#!/bin/bash

# Check if ag_gen exists
psql -lqt | cut -d \| -f 1 | grep -qw ag_gen
if [ $? -eq 0 ]; then
    dropdb ag_gen
fi

createdb ag_gen

psql ag_gen < sql/create_ag_schema.sql
psql ag_gen < sql/ag_example_data.sql
