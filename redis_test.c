#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_redisconnect.h"
#include "ag_redisclient.h"

int main()
{
	redisContext *c;

	c = malloc(sizeof(redisContext*));
	c = RedisConnect(c);

	RedisSetContext(c);
	RedisPing();
	RedisFree(c);

	return 0;
}
