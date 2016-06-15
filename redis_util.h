#ifndef C_R_CLIENT_H
#define C_R_CLIENT_H

#include <hiredis/hiredis.h>

#define DELIMITER ":"

int RedisConnect(void);
int RedisDisconnect(void);
int RedisPing(void);
int RedisEnqueueValue(const char *key, const char *value);
char *RedisDequeueValue(const char *key);
int RedisListLength(const char *key);
void RedisReplyError(redisReply *);

#endif //C_R_CLIENT_H
