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
		bool connected = false;
	public:
		Connection(void) {}
		Connection(std::string conninfo) {
			connect(conninfo);
		}

		void connect(std::string conninfo) {
			// Create database connection
			conn_r = PQconnectdb(conninfo.c_str());
			if(PQstatus(conn_r) != CONNECTION_OK) {
				throw DBException("Database connection failed.");
			}
			connected = true;
		}

		bool is_connected(void) {
			return connected;
		}

		void close() {
			// Close connection before exiting
			if(conn_r) {
				PQfinish(conn_r);
			}
			connected = false;
		}

		friend class DB;
	};

	Connection conn;
public:
	typedef std::vector<std::string> Row;
	static DB instance;

	static DB& connect(std::string conninfo) {
		if(!instance.conn.is_connected()) {
			instance.conn.connect(conninfo);
		}
		return instance;
	}

	static void close(void) {
		instance.conn.close();
	}

	static std::vector<Row> exec(std::string sql) {
		if(!instance.conn.is_connected()) {
			throw DBException("Not connected to Database.");
		}

		PGresult* res = PQexec(instance.conn.conn_r, sql.c_str());
		if(PQresultStatus(res) != PGRES_TUPLES_OK) {
			throw DBException(PQerrorMessage(instance.conn.conn_r));
		}

		int numrows = PQntuples(res);
		int numfields = PQnfields(res);
		std::vector<Row> rows;
		for(auto i=0; i<numrows; i++) {
			Row new_row;
			for(auto j=0; j<numfields; j++) {
				new_row.push_back(PQgetvalue(res, i, j));
			}
			rows.push_back(new_row);
		}

		PQclear(res);
		return rows;
	}

	DB(DB const&) = delete;
	void operator=(DB const&) = delete;
};

#endif //UTIL_DB_HPP
