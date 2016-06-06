#ifndef C_AG_REDISCLIENT_H
#define C_AG_REDISCLIENT_H

#include <hiredis/hiredis.h>
#include "ag_asset.h"

int RedisSetContext(redisContext *);
int RedisSetListName(char*);
void RedisPing(void);
int RedisEnqueueAsset(struct AGAsset *);
struct AGAsset *RedisDequeueAsset(void);
int RedisQueueLength(void);

#endif //C_AG_REDISCLIENT_H
