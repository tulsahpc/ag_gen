#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "identifier.h"
#include "network_state.h"
#include "exploit.h"
#include "ag_generator_utils.h"
#include "globals.h"

int num_exploits;
struct identifier_hashtable *identifier_ht;
struct asset_hashtable *asset_ht;
struct fact_hashtable *fact_ht;
struct network_state *ns;
struct exploit_list_node *exploits;

extern FILE *nsin, *epin;

int nslex();
int nsparse();

int eplex();
int epparse();

int dot_to_file(char *, char *);

void odometer(int, int, char **, char **);

char *od, *global_str;
char *input;
char *output;

void simple_test(char **, char **);

struct bound_param_lists *b_pl;

int main(int argc, char **argv) {
    // MAKE THE IDENTIFIER HASHTABLE
    identifier_ht = (struct identifier_hashtable *) malloc(sizeof(struct identifier_hashtable));
    identifier_ht->size = M_id;
    identifier_ht->head = (struct identifier_hashtable_node **) calloc(M_id, M_id *sizeof(struct identifier_hashtable_node *));

    asset_ht = make_asset_hashtable(asset_ht);
    fact_ht = make_fact_hashtable();
    ns = new_network_state(ns, asset_ht, fact_ht);

//    // READ IN NETWORK STATE
//    if (argc > 0) {
//        printf("Reading in Network Model:%s\n", argv[1]);
//        nsin = fopen(argv[1], "r");
//        nsparse();
//        printf("Done parsing\n");
//        printf("Caggen network state; s - %p\n", (void *) ns);
//        printf("Caggen network state; a - %p; f - %p\n", (void *) ns->a_ht, (void *) ns->f_ht);
//        printf("%s\n", network_state_to_string(ns));
//    } else {
//        printf("You must specify a network model file\n.");
//    }
//
//    // READ IN EXPLOITS
//    if (argc > 1) {
//        printf("Reading in Exploit Patterns:%s\n", argv[2]);
//        epin = fopen(argv[2], "r");
//        epparse();
//        printf("%s\n", exploit_list_to_string(exploits));
//    }
//    else printf("You must specify an exploits file.\n");

    b_pl = exploit_asset_bindings(asset_ht, exploits->e);
    printf("HAPPY AND DONE.\n");

    char *seed = strdup("");
    odometer(4, 3, &seed, &od);
    printf("%s\n", od);

    printf("AFTER ODOMETER HAPPY AND DONE.\n");
}

void odometer(int max_digits, int rollers, char **base_str, char **ret_str) {
    char *tmp_str, *result_str, *new_base_str;
    int digits = max_digits;
    if (rollers >= 0) {
        asprintf(&tmp_str, "");
        while (digits != 0) {
            asprintf(&new_base_str, "%s%d", *base_str, digits);
            odometer(max_digits, rollers - 1, &new_base_str, &result_str);
            digits = digits - 1;
            if (rollers == 0) asprintf(&tmp_str, "%s\n%s", result_str, tmp_str);
            else asprintf(&tmp_str, "%s%s", result_str, tmp_str);
        }
        *ret_str = tmp_str;
    }
    else {
        *ret_str = *base_str;
    }
}

void simple_test(char **in, char **out) {
    printf("in simple_test:before in=%s\n", *in);
    printf("in simple_test:before out=%s\n", *out);
    *out = *in;
    printf("in simple_test:out out=%s\n", *out);
}

int dot_to_file(char *s, char *fname) {
    FILE *fptr;
    fptr = fopen(fname, "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    else {
        fprintf(fptr, "%s", s);
        fclose(fptr);
        return 0;
    }
}
