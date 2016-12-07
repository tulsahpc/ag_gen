/** \file db_util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <cstdlib>
#include <cstring>
#include <libpq-fe.h>

#include "util_common.hpp"
#include "util_db.hpp"

PGconn *conn;
// Returns 0 on success, any other number on error.
int dbconnect(const char *conninfo)
{
	// Create database connection
	conn = PQconnectdb(conninfo);
	if(PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
		goto fatal;
	}
fatal:
	//dbclose();
	return -1;
}

void dbclose()
{
	// Close connection before exiting
	if(conn) PQfinish(conn);
}

int dbtrans_begin()
{
	PGresult *res;

	// Create transaction
	res = PQexec(conn, "BEGIN");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
		goto fatal;
	}
	PQclear(res);
	return 0;
fatal:
	PQclear(res);
	return -1;
}

int dbtrans_end()
{
	PGresult *res;

	// Finish transaction
	res = PQexec(conn, "COMMIT");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "COMMIT failed: %s", PQerrorMessage(conn));
		goto fatal;
	}

	PQclear(res);
	return 0;
fatal:
	PQclear(res);
	return -1;
}

void printResult(const PGresult *res)
{
	int rows = PQntuples(res);
	int cols = PQnfields(res);
	int colWidths[cols];

	DEBUG_PRINT("Rows: %d\n", rows);
	DEBUG_PRINT("Columns: %d\n", cols);

	// Get the maximum length of each column
	for(int i=0; i<cols; i++) {
		int colMax = strlen(PQfname(res, i));
		for(int j=0; j<rows; j++) {
			int nextVal = PQgetlength(res, j, i);
			if(nextVal > colMax){
				colMax = nextVal;
			}
		}
		colWidths[i] = colMax+1;
	}

	// Print the column headers
	for(int i=0; i<cols; i++) {
		printf("|%*s ", colWidths[i], PQfname(res, i));
	}
	printf("|\n");

	// Print each row
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			printf("|%*s ", colWidths[j], PQgetvalue(res, i, j));
		}
		printf("|\n");
	}
	printf("\n");
}
