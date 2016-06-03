#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_redisconnect.h"

#define HOSTNAME "127.0.0.1"
#define PORT 6379

redisContext* RedisConnect(redisContext *c) 
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

	return c;
}

int RedisFree(redisContext *c)
{
	redisFree(c);
	return 0;
}
