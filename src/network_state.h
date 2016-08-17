#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "asset.h"
#include "util_list.h"

struct NetworkState {
	int network_id;
	struct NetworkState *parent;
	struct List *children;
	struct List *assets;
};

struct NetworkState *ns_new(int id, struct NetworkState * parent);
void ns_free(struct NetworkState *);

int ns_addchild(struct NetworkState *state, struct NetworkState *child);
int ns_populateassets(struct NetworkState *state);

#endif
