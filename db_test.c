#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define DEBUG_PRINT(fmt, args...) \
    do { if (DEBUG_TEST) fprintf(stderr, "DEBUG: %s:%d %s(): " fmt, __FILE__, \
    __LINE__, __FUNCTION__, ##args); } while (0)

static void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

int main() {
    const char *conninfo;
    PGconn *conn;
    PGresult *res;

    // Database connection information
    conninfo = "postgresql://localhost:5432/ag_gen";

    // Create database connection
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    // Create transaction
    res = PQexec(conn, "BEGIN");
    if(PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    // Reinitialize result struct
    PQclear(res);

    res = PQexec(conn, "SELECT * FROM asset");
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    DEBUG_PRINT("Rows: %d\n", rows);
    DEBUG_PRINT("Columns: %d\n", cols);

    // Get the maximum length of each column
    int colWidths[cols];
    for(int i=0; i<cols; i++) {
      int colMax = (int)strlen(PQfname(res, i));
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

  PQclear(res);

  // Finish transaction
  res = PQexec(conn, "COMMIT");
  if(PQresultStatus(res) != PGRES_COMMAND_OK) {
      fprintf(stderr, "COMMIT failed: %s", PQerrorMessage(conn));
      PQclear(res);
      exit_nicely(conn);
  }

  // Close connection before exiting
  PQfinish(conn);
}
