#ifndef C_AG_REDISCLIENT_H
#define C_AG_REDISCLIENT_H

#include <hiredis/hiredis.h>
#include "ag_asset.h"

// char *assetlistkey; //name of list in redis
// redisContext *c; //context of the redis server so the rest of the program doesn't need it
// redisReply *reply;

int RedisSetContext(redisContext *);
int RedisSetListName(char*);
void RedisPing(void);
int RedisEnqueueAsset(struct AGAsset *);
struct AGAsset *RedisDequeueAsset(void);
int RedisQueueLength(void);

#endif //C_AG_REDISCLIENT_H
