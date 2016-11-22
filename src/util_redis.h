/** \file redis_util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_REDIS_H
#define UTIL_REDIS_H

#ifdef __c_plus_plus
extern "C" {
#endif

#include <hiredis/hiredis.h>

#define DELIMITER ":"

int redis_connect(char *, int);
int redis_disconnect(void);
int redis_ping(void);
int redis_enqueue(const char *key, const char *value);
char *redis_dequeue(const char *key);
int rlist_length(const char *key);
void rreply_error(redisReply *);

#ifdef __c_plus_plus
}
#endif

#endif //UTIL_REDIS_H