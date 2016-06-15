#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "redis_util.h"
#include "util.h"

#define HOSTNAME "127.0.0.1"
#define PORT 6379

redisContext *ctx;

int RedisConnect()
{
	//uses default hostname and port instead of being taken from argc
	//const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
	//int port = (argc > 2) ? atoi(argv[2]) : 6379;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	ctx = redisConnectWithTimeout(HOSTNAME, PORT, timeout);
	if (ctx == NULL || ctx->err) {
		if (ctx) {
			printf("Connection error: %s\n", ctx->errstr);
			redisFree(ctx);
		} else {
			printf("Connection error: can't allocate redis context\n");
		}
		return 1;
	}

	return 0;
}

int RedisDisconnect()
{
	redisFree(ctx);
	return 0;
}

int RedisPing()
{
	redisReply *reply = redisCommand(ctx, "PING");

	if(reply == NULL) {
		RedisReplyError(reply);
		return 1;
	}

	printf("PING %s\n", reply->str);
	freeReplyObject(reply);

	return 0;
}


int RedisEnqueueValue(const char *key, const char* value)
{
	redisReply *reply = redisCommand(ctx, "LPUSH %s %s", key, value);
	if(reply == NULL) {
		RedisReplyError(reply);
		return 1;
	}
	freeReplyObject(reply);
	return 0;
}

char *RedisDequeueValue(const char *key)
{
	if(key == NULL)
		return NULL;

	redisReply *reply = redisCommand(ctx, "RPOP %s", key);
	if(reply == NULL) {
		RedisReplyError(reply);
		freeReplyObject(reply);
		return NULL;
	}

	DEBUG_PRINT("reply type: %d\n", reply->type);
	if(reply->type == REDIS_REPLY_NIL) {
		RedisReplyError(reply);
		freeReplyObject(reply);
		return NULL;
	}

	char *value = dynstr(reply->str);
	freeReplyObject(reply);

	return value;
}

int RedisListLength(const char *key)
{
	int len = 0;
	redisReply *reply = redisCommand(ctx, "LLEN %s", key);

	if(reply == NULL) {
		RedisReplyError(reply);
		freeReplyObject(reply);
		return -1;
	}

	len = atoi(reply->str);
	freeReplyObject(reply);

	return len;
}

void RedisReplyError(redisReply *reply)
{
	printf("Redis error: %s\n", ctx->errstr);
	freeReplyObject(reply);
	redisFree(ctx);
}
