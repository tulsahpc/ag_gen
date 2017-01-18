/** \file redis_util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 *
 * Handles the redis server.
 *
 * Has all of the necessary helper functions for setting up a server context,
 * disconnecting and freeing a contexts resources, pinging the server, adding data into a queue,
 * getting data back out of a queue, and getting the length of any particular redis list.
 *
 * Is responsible for managing the context and redis replies.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <hiredis/hiredis.h>

#include "util_common.hpp"
#include "util_redis.hpp"

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
int redis_connect(char *hostname, int port)
{
	//uses default hostname and port instead of being taken from argc
	//const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
	//int port = (argc > 2) ? atoi(argv[2]) : 6379;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	ctx = redisConnectWithTimeout(hostname, port, timeout);
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
int redis_disconnect()
{
	redisFree(ctx);
	return 0;
}

/**
 * Pings the redis server.
 *
 * It prints the pong to the console.
 * If the ping fails it will call rreply_error and return a 1.
 */
int redis_ping()
{
	redisReply *reply = ( redisReply * ) redisCommand( ctx, "PING");

	if(reply == NULL) {
		rreply_error(reply);
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
 * If it fails it calls rreply_error and returns 1.
 */
int redis_enqueue(const char *key, const char* value)
{
	redisReply *reply = ( redisReply * ) redisCommand( ctx, "LPUSH %s %s", key, value);
	if(reply == NULL) {
		rreply_error(reply);
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
char *redis_dequeue(const char *key)
{
	if(key == NULL)
		return NULL;

	redisReply *reply = ( redisReply * ) redisCommand( ctx, "RPOP %s", key);
	if(reply == NULL) {
		rreply_error(reply);
		freeReplyObject(reply);
		return NULL;
	}

	if(reply->type == REDIS_REPLY_NIL) {
		rreply_error(reply);
		freeReplyObject(reply);
		return NULL;
	}

	char *value = dynstr(reply->str);
	freeReplyObject(reply);

	return value;
}

/**
 * Gets the length of a list given by key.
 *
 * If key returns a null list then it calls rreply_error and returns -1.
 */
int rlist_length(const char *key)
{
	int len = 0;
	redisReply *reply = ( redisReply * ) redisCommand( ctx, "LLEN %s", key);

	if(reply == NULL) {
		rreply_error(reply);
		freeReplyObject(reply);
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
void rreply_error(redisReply *reply)
{
	printf("Redis error: %s\n", ctx->errstr);
	freeReplyObject(reply);
	redisFree(ctx);
}
