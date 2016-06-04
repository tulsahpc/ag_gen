#ifndef C_AG_REDISCONNECT_H
#define C_AG_REDISCONNECT_H

#include <hiredis/hiredis.h>

struct redisContext *RedisConnect(redisContext *c);
int RedisFree(redisContext *c);

#endif //C_AG_REDISCONNECT_H
