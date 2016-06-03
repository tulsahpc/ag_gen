#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_redisconnect.h"
#include "ag_asset.h"
#include "ag_redisclient.h"


int main()
{
	redisContext *c;

	c = malloc(sizeof(int*));
	c = RedisConnect(c);

	RedisSetContext(c);

	printf("Ping %s\n", RedisPing());

	RedisFree(c);

	return 0;
}