struct Permutations {
	int size;
	int k;
	int n;
	int **arr;
};

struct Permutations *odometer(int k, int n);
void PermutationsFree(struct Permutations *);
void PermutationsPrint(struct Permutations *);
void PermutationsPrintSet(struct Permutations *, char **set);
