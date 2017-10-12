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
#include <memory>
#include <libpq-fe.h>

class DB;

extern std::shared_ptr<DB> db;

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
            if (PQstatus(conn_r) != CONNECTION_OK) {
                throw DBException("Database connection failed.");
            }
            connected = true;
        }

        bool is_connected(void) {
            return connected;
        }

        void close() {
            // Close connection before exiting
            if (conn_r) {
                PQfinish(conn_r);
            }
            connected = false;
        }

        friend class DB;
    };

    Connection conn;
public:
    typedef std::vector<std::string> Row;

    DB() {};

    DB(std::string conninfo) : conn(conninfo) {};

    std::vector<Row> exec(std::string sql) {
        if (!conn.is_connected()) {
            throw DBException("Not connected to Database.");
        }

        std::vector<Row> rows;
        PGresult *res = PQexec(conn.conn_r, sql.c_str());
        if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            // No return
        } else if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            // Return rows
            int numrows = PQntuples(res);
            int numfields = PQnfields(res);
            for (auto i = 0; i < numrows; i++) {
                Row new_row;
                for (auto j = 0; j < numfields; j++) {
                    new_row.push_back(PQgetvalue(res, i, j));
                }
                rows.push_back(new_row);
            }
        } else {
            // DB Error
            throw DBException(PQerrorMessage(conn.conn_r));
        }

        PQclear(res);
        return rows;
    }

    void close() {
        conn.close();
    }
};

#endif //UTIL_DB_HPP
