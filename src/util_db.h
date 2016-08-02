/** \file db_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_DB_H
#define UTIL_DB_H

#include <libpq-fe.h>

extern PGconn *conn;

void dbconnect(const char *);
void dbclose(void);
void dbtrans_begin(void);
void dbtrans_end(void);
void exit_nicely(void);
void printResult(const PGresult*);

#endif //UTIL_DB_H
