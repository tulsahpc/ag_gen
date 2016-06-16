/** \file db_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef C_DB_UTIL_H
#define C_DB_UTIL_H

#include <libpq-fe.h>

extern PGconn *conn;

void AGDbConnect(const char *);
void AGDbDisconnect(void);
void AGDbBeginTransaction(void);
void AGDbEndTransaction(void);

void printResult(const PGresult*);

void exit_nicely(void);

#endif //C_DB_UTIL_H
