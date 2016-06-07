#ifndef C_R_CLIENT_H
#define C_R_CLIENT_H

#include <hiredis/hiredis.h>

struct RList {
	redisContext *c;
	char *key;
}

int RedisConnect(redisContext *c);
int RedisContextFree(redisContext *c);
void RedisPing(redisContext *c);
int RedisEnqueueValue(redisContext *c, char *, char *)
char *RedisDequeueValue(redisContext *c, char *);
int RedisListLength(redisContext *c, char *);

#endif //C_R_CLIENT_H
