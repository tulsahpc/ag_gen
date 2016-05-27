//
// Created by Kyle Cook on 5/26/16.
//

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
