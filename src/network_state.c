#include <stdlib.h>
#include "network_state.h"

struct NetworkState *ns_new(int id, struct NetworkState *parent)
{
	struct NetworkState *new_state = malloc(sizeof(struct NetworkState));
	new_state->network_id = id;
	new_state->parent = parent;
	new_state->children = list_new();
	new_state->assets = list_new();
	return new_state;
}

void ns_free(struct NetworkState *state)
{
	list_free(state->children);
	list_free(state->assets);
	free(state);
}
