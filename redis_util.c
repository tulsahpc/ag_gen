/**
 * Handles the redis server.
 *
 * Has all of the necessary helper functions for setting up a server context,
 * disconnecting and freeing a contexts resources, pinging the server, adding data into a queue,
 * getting data back out of a queue, and getting the length of any particular redis list.
 *
 * Is responsible for managing the context and redis replies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "redis_util.h"
#include "util.h"

#define MAXSTRLEN 128

/**
 * Default hostname and port of a redis server.
 *
 * To be changed later to allow for editable hostname and port numbers.
 */
#define HOSTNAME "127.0.0.1"
#define PORT 6379


/**
 * Global redis server context.
 *
 * Currently only one redis server can be accessed while the program is running.
 * Should be changed in the future to allow multiple redis server's to be accessed at runtime.
 */
redisContext *ctx;


/**
 * Connects to a redis server.
 *
 * Using the global context this function looks for and connects to the redis server given by the hostname and port
 *
 * If a connection cannot be made this functions prints to the console and returns 1.
 */
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

/**
 * Frees up the resources a redis context uses.
 *
 *
 */
int RedisDisconnect()
{
	redisFree(ctx);
	return 0;
}

/**
 * Pings the redis server.
 *
 * It prints the pong to the console.
 * If the ping fails it will call RedisReplyError and return a 1.
 */
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

/**
 * Enqueues the given value to a redis list given by key.
 *
 * Pushes the given value to a redis list given by the key.
 * If it fails it calls RedisReplyError and returns 1.
 */
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

/**
 * Dequeues the topmost value from a redis list given by key.
 *
 * If the key refers to a null list then it is printed to the console and returns null.
 *
 */
char *RedisDequeueValue(const char *key)
{
	char *value = malloc(sizeof(char)*(MAXSTRLEN+1));
	redisReply *reply = redisCommand(ctx, "RPOP %s", key);

	if(reply == NULL) {
		RedisReplyError(reply);
		return NULL;
	}

	DEBUG_PRINT("reply type: %d\n", reply->type);
	if(reply->type == REDIS_REPLY_NIL) {
		printf("%s does not exist.\n", key);
		return NULL;
	}

	int len = reply->len;
	strncpy(value, reply->str, len);

	if(len < MAXSTRLEN) {
		value[len] = '\0';
	} else {
		value[MAXSTRLEN] = '\0';
	}

	freeReplyObject(reply);
	return value;
}

/**
 * Gets the length of a list given by key.
 *
 * If key returns a null list then it calls RedisReplyError and returns -1.
 */
int RedisListLength(const char *key)
{
	int len = 0;
	redisReply *reply = redisCommand(ctx, "LLEN %s", key);

	if(reply == NULL) {
		RedisReplyError(reply);
		return -1;
	}

	len = atoi(reply->str);
	freeReplyObject(reply);
	return len;
}

/**
 * Gets called whenever there is an error in the redis server reply.
 *
 * Prints out the error to the context and frees the server from its context.
 * Will cause the program to no longer funtion if this is called.
 *
 */
void RedisReplyError(redisReply *reply)
{
	printf("Redis error: %s\n", ctx->errstr);
	freeReplyObject(reply);
	redisFree(ctx);
}
