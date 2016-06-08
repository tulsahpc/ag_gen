#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "r_client.h"

#define MAXSTRLEN 128

#define HOSTNAME "127.0.0.1"
#define PORT 6379

int RCListGet(struct RCList *list)
{

}

int RedisConnect(redisContext *c)
{
	//uses default hostname and port instead of being taken from argc
	//const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
	//int port = (argc > 2) ? atoi(argv[2]) : 6379;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	c = redisConnectWithTimeout(HOSTNAME, PORT, timeout);
	if (c == NULL || c->err) {
		if (c) {
			printf("Connection error: %s\n", c->errstr);
			redisFree(c);
		} else {
			printf("Connection error: can't allocate redis context\n");
		}
		exit(1);
	}

	return 0;
}

int RedisContextFree(redisContext *c)
{
	redisFree(c);
	return 0;
}

void RedisPing(redisContext *c)
{
	redisReply *reply = redisCommand(c,"PING");
	printf("PING %s\n", reply->str);
	freeReplyObject(reply);
}


int RedisEnqueueValue(redisContext *c, char *key, char* value)
{
	redisReply *reply = redisCommand(c, "LPUSH %s %s", key, value);
	freeReplyObject(reply);
	return 0;
}

char * RedisDequeueValue(redisContext *c, char *key)
{
	char *value = malloc(sizeof(char)*MAXSTRLEN);
	redisReply *reply = redisCommand(c, "RPOP %s", key);
	strcpy(value,reply->str);
	freeReplyObject(reply);
	return value;
}

int RedisListLength(redisContext *c, char *key)
{
	int len = 0;
	redisReply *reply = redisCommand(c, "LLEN %s", key);
	len = atoi(reply->str);
	freeReplyObject(reply);
	return len;
}
