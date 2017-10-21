#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
        #include <libpq-fe.h>
#elif __linux__
        #include <postgresql/libpq-fe.h>
#endif

void checkSelect(PGconn *conn, PGresult *res) {
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}
}
std::vector< std::vector<int> > create_matrix(const char *conninfo) {
	
	PGconn		*conn;
	PGresult	*res;
	
	conn = PQconnectdb(conninfo);
	if (PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	
	res = PQexec(conn, "SELECT * FROM factbase;");
	checkSelect(conn, res);
	// will the factbase have integer or long rows?
	int num_factbases =  PQntuples(res);


	std::vector< std::vector<int> > matrix;
	for (int i=0; i<num_factbases; i++) {
		std::vector<int> temp (num_factbases, 0);
		matrix.push_back(temp);
	}
	PQclear(res);
	
	// Assumes factbase id goes from 0 to number of factbases -1
	res = PQexec(conn, "SELECT * FROM edge;");
	checkSelect(conn, res);
	int edges = PQntuples(res);
	for (int i=0; i<edges; i++) {
		int a = atoi( PQgetvalue(res,i,1) );
		int b = atoi( PQgetvalue(res,i,2) );
		matrix[a][b] = 1;
	}
	PQclear(res);
	return matrix;	
}

/*int main() {
	const char *conninfo;
	conninfo = "dbname=ag_gen_test host=localhost user=kyle";
	std::vector< std::vector<int> > matrix = create_matrix(conninfo);
	int n = matrix.size();
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}	
	return 0;
}
*/
