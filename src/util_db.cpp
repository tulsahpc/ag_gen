/** \file db_util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <cstdlib>
#include <string>
#include <cstring>
#include <libpq-fe.h>

#include "util_common.hpp"
#include "util_db.hpp"

using namespace std;

PGconn *conn;
void dbconnect(string conninfo)
{
	// Create database connection
	conn = PQconnectdb(conninfo.c_str());
	if(PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
		exit(1);
	}
}

void dbclose()
{
	// Close connection before exiting
	if(conn) PQfinish(conn);
}

void dbtrans_begin(void)
{
	PGresult *res;

	// Create transaction
	res = PQexec(conn, "BEGIN");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit(1);
	}
	PQclear(res);
}

void dbtrans_end(void)
{
	PGresult *res;

	// Finish transaction
	res = PQexec(conn, "COMMIT");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "COMMIT failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit(1);
	}

	PQclear(res);
}

void printResult(const PGresult *res)
{
	int rows = PQntuples(res);
	int cols = PQnfields(res);
	int colWidths[cols];

	// Get the maximum length of each column
	for(int i=0; i<cols; i++) {
		string columnName = PQfname(res,i);
		int colMax = columnName.size();
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
