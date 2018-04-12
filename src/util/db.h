/** \file db_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved.
 * Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_DB_HPP
#define UTIL_DB_HPP

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <libpq-fe.h>

class DB;
extern std::shared_ptr<DB> db;
typedef std::vector<std::string> Row;

class DBException : public std::runtime_error {
  public:
    explicit DBException(std::string &error_message)
        : std::runtime_error(error_message) {}
    explicit DBException(std::string &&error_message)
        : std::runtime_error(error_message) {}
};

class Connection {
    bool connected = false;

    PGconn *conn_r;

  public:
    Connection(const std::string &conninfo) {
        // Create database connection
        conn_r = PQconnectdb(conninfo.c_str());
        if (PQstatus(conn_r) != CONNECTION_OK) {
            std::string errormsg(PQerrorMessage(conn_r));
            throw DBException("Database connection failed: " + conninfo + "\n" +
                              errormsg);
        }
        connected = true;
    }

    ~Connection() {
        // Close connection before exiting
        if (conn_r) {
            PQfinish(conn_r);
        }
        connected = false;
    }

    bool is_connected() { return connected; }

    std::vector<Row> exec(const std::string &sql) {
        if (!is_connected()) {
            throw DBException("Not connected to Database.");
        }

        std::vector<Row> rows;
        PGresult *res = PQexec(conn_r, sql.c_str());
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
            throw DBException(PQerrorMessage(conn_r));
        }

        PQclear(res);
        return rows;
    }
};

class DB {
    Connection conn;

public:
    DB(const std::string &conninfo) : conn(conninfo) {}

    void connect(const std::string &conninfo) {
        conn = Connection {conninfo};
    }

    std::vector<Row> exec(const std::string &sql) {
        try {
            auto results = conn.exec(sql);
            return results;
        } catch (DBException &e) {
            std::cerr << "Database Exception: " << e.what() << std::endl;
            abort();
        }
    }
};

#endif // UTIL_DB_HPP
