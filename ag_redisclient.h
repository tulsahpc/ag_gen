char* assetlistkey; //name of list in redis
redisContext *c; //context of the redis server so the rest of the program doesn't need it
redisReply *reply; 

int RedisSetContext(redisContext *context);
int RedisSetListName(char* name);
char* RedisPing();
int RedisEnqueueAsset(struct AGAsset *asset);
struct AGAsset* RedisDequeueAsset();
int RedisQueueLength();