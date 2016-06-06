#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_asset.h"
#include "ag_redisclient.h"
#include "util.h"

#define MAXSTRLEN 128
#define DELIMITERS ":"

char *assetlistkey;
redisContext *c;

int RedisSetContext(redisContext *context)
{
	c = context;
	return 0;
}

int RedisSetListName(char* name)
{
	assetlistkey = malloc(sizeof(char)*MAXSTRLEN);
	assetlistkey = name;
	return 0;
}

void RedisPing()
{
	redisReply *reply = redisCommand(c,"PING");
	printf("PING %s\n", reply->str);
	freeReplyObject(reply);
}

int RedisEnqueueAsset(struct AGAsset *asset)
{
	redisReply *reply = redisCommand(c, "LPUSH %s %d:%s:%d", assetlistkey, asset->id, asset->name, asset->network_id);
	freeReplyObject(reply);
	return 0;
}

struct AGAsset* RedisDequeueAsset()
{
	char *assetstr = malloc(sizeof(char)*MAXSTRLEN);
	char *saveptr;
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));

	redisReply *reply = redisCommand(c, "RPOP %s", assetlistkey);
	assetstr = strncpy(assetstr,reply->str, reply->len);
	assetstr[reply->len] = '\0';
	freeReplyObject(reply);

	if(assetstr != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", assetstr);

		asset->id = atoi(strtok_r(assetstr, DELIMITERS, &saveptr));
		asset->name = strtok_r(NULL, DELIMITERS, &saveptr);
		asset->network_id = atoi(strtok_r(NULL, DELIMITERS, &saveptr));
	}
	free(assetstr);
	return asset;
}

int RedisQueueLength()
{
	int len = 0;
	redisReply *reply = redisCommand(c, "LLEN %s", assetlistkey);
	len = atoi(reply->str);
	freeReplyObject(reply);
	return len;
}
