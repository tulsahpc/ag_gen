#ifndef UTIL_ODOMETER_H
#define UTIL_ODOMETER_H

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

struct Odometer *odometer_new(int k, int n);
void odometer_free(struct Odometer *);
void odometer_print(struct Odometer *);
void odometer_printset(struct Odometer *, char **set);

struct OdometerState *ostate_new(struct Odometer *od);
int *ostate_next(struct OdometerState *state);
void ostate_free(struct OdometerState *state);

#endif //UTIL_ODOMETER_H
