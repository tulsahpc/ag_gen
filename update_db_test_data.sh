#!/bin/bash

psql ag_gen < sql/delete_ag_schema.sql
psql ag_gen < sql/create_ag_schema.sql
psql ag_gen < sql/ag_example_data.sql
