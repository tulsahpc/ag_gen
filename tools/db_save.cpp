#include "../src/util_db.h"

int main(int argc, char **argv) {
	dbconnect("ag_gen_test@localhost");
	dbtrans_begin();

	PGresult* res;
	res = PQexec(conn, "INSERT INTO factbase VALUES (1, 25)");

	printf("%s\n", PQresStatus(PQresultStatus(res)));

	dbtrans_end();
	dbclose();
}
