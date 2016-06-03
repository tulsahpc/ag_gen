#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_redisconnect.h"
#include "ag_asset.h"

int main()
{
	redisContext *c;
	redisReply *reply;

	c = malloc(sizeof(int*));
	c = RedisConnect(c);

	/* PING server */
	reply = redisCommand(c,"PING");
	printf("PING: %s\n", reply->str);
	freeReplyObject(reply);

	RedisFree(c);

	return 0;
}