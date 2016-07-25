#ifndef C_UTIL_ODOMETER_H
#define C_UTIL_ODOMETER_H

struct Odometer {
	int size;
	int k;
	int n;
	int **arr;
};

struct OdometerState {
	struct Odometer *od;
	int idx;
};

struct Odometer *OdometerNew(int k, int n);
void OdometerFree(struct Odometer *);
void OdometerPrint(struct Odometer *);
void OdometerPrintSet(struct Odometer *, char **set);

struct OdometerState *initOdometerState(struct Odometer *od);
int *nextPermutation(struct OdometerState *state);

#endif //C_UTIL_ODOMETER_H
