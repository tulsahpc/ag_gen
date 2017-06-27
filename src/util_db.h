/** \file db_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_DB_HPP
#define UTIL_DB_HPP

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <libpq-fe.h>

class DBException : public std::runtime_error {
public:
	DBException(std::string error_message) : std::runtime_error(error_message) {}
};

class DB {
	class Connection {
		PGconn *conn_r;
	public:
		Connection(std::string conninfo) {
			// Create database connection
			conn_r = PQconnectdb(conninfo.c_str());
			if(PQstatus(conn_r) != CONNECTION_OK) {
				throw DBException("Database connection failed.");
			}
		}

		void close() {
			// Close connection before exiting
			if(conn_r) {
				PQfinish(conn_r);
			}
		}

		friend class DB;
	};

	create_tuple

	Connection conn;
public:
	typedef std::vector<std::string> Row;
	DB(std::string conninfo) : conn(conninfo) {}

	std::vector<Row> exec(std::string sql) {
		PGresult* res = PQexec(conn.conn_r, sql.c_str());
		if(PQresultStatus(res) != PGRES_TUPLES_OK) {
			throw DBException(PQerrorMessage(conn.conn_r));
		}

		int numrows = PQntuples(res);
		int numfields = PQnfields(res);
		for(auto i=0; i<numrows; i++) {

		}

		PQclear(res);
	}
};

#endif //UTIL_DB_HPP
