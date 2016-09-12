/** \file db_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_DB_H
#define UTIL_DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libpq-fe.h>

extern PGconn *conn;

int dbconnect(const char *);
void dbclose(void);
int dbtrans_begin(void);
int dbtrans_end(void);
void printResult(const PGresult*);

#ifdef __cplusplus
}
#endif

#endif //UTIL_DB_H
