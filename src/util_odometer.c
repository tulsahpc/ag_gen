#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "util.h"
#include "util_odometer.h"

static int elts;
static int len;
static int *arr;
static int cur = 0;

static int can_inc(int idx)
{
	if(idx < len && arr[idx] < elts-1)
		return 1;
	return 0;
}

static void inc(int idx)
{
	if(idx < len)
		arr[idx]++;
}

static void reset(int idx)
{
	for(int i=0; i<idx; i++)
		arr[i] = 0;
	cur = 0;
}

struct Odometer *OdometerNew(int k, int n)
{
	elts = k;
	len = n;
	arr = calloc(1, n * sizeof(int));
	struct Odometer *perms = malloc(sizeof(struct Odometer));
	int num_perms = pow(k, n);
	int **perm_arr = malloc(num_perms * sizeof(int*));

	int curr_perm = 0;
	while(cur < len) {
		perm_arr[curr_perm] = malloc(len * sizeof(int));
		for(int i=0; i<len; i++) {
			perm_arr[curr_perm][i] = arr[i];
		}
		curr_perm++;

		if(can_inc(cur)) {
			inc(cur);
		} else {
			while(cur < len && !can_inc(cur)) {
				cur++;
			}
			if(can_inc(cur))
				inc(cur);
			else
				break;
			reset(cur);
		}
	}

	perms->arr = perm_arr;
	perms->size = num_perms;
	perms->n = n;
	perms->k = k;

	free(arr);

	return perms;
}

void OdometerFree(struct Odometer *perm)
{
	for(int i=0; i<perm->size; i++) {
		free(perm->arr[i]);
	}
	free(perm->arr);
	free(perm);
}

void OdometerPrint(struct Odometer *perm)
{
	for(int i=0; i<perm->size; i++) {
		for(int j=0; j<perm->n; j++) {
			printf("%d ", perm->arr[i][j]);
		}
		printf("\n");
	}
}

void OdometerPrintSet(struct Odometer *perm, char **set)
{
	for(int i=0; i<perm->size; i++) {
		for(int j=0; j<perm->n; j++) {
			printf("%s ", set[perm->arr[i][j]]);
		}
		printf("\n");
	}
}

struct OdometerState *initOdometerState(struct Odometer *od)
{
	struct OdometerState *state = malloc(sizeof(struct OdometerState));
	state->od = od;
	state->idx = 0;

	return state;
}

int *nextPermutation(struct OdometerState *state)
{
	int **arr = state->od->arr;
	return arr[state->idx++];
}

void freeOdometerState(struct OdometerState *state)
{
	free(state);
}
