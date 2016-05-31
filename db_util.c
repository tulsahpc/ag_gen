#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "util.h"
#include "db_util.h"

PGconn *conn;
void AGDbConnect(const char *conninfo)
{
	// Create database connection
	conn = PQconnectdb(conninfo);
	if(PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
		exit_nicely();
	}
}

void AGDbDisconnect()
{
	// Close connection before exiting
	PQfinish(conn);
}

void AGDbBeginTransaction()
{
	PGresult *res;

	// Create transaction
	res = PQexec(conn, "BEGIN");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}
	PQclear(res);
}

void AGDbEndTransaction()
{
	PGresult *res;

	// Finish transaction
	res = PQexec(conn, "COMMIT");
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "COMMIT failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}

	PQclear(res);
}

void exit_nicely(void)
{
	PQfinish(conn);
	exit(1);
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
