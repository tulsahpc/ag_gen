// network state data structures and function declaration

#ifndef NETWORK_STATE_H_
#define NETWORK_STATE_H_

#define QUAL 101
#define TOPO 102

#define M_a 101
#define M_f 101

struct asset {
    struct identifier *name;
};

struct asset_hashtable_node {
    struct asset *a;
    struct asset_hashtable_node *next;
};

struct asset_hashtable {
    int size;
    struct asset_hashtable_node **head;
};

struct quality {
    struct asset *a;
    struct identifier *property;
    struct identifier *value;
};

struct topology {
    struct asset *a1;
    struct asset *a2;
    struct identifier *relationship;
};

struct fact {
    int type;
    union {
        struct quality *q;
        struct topology *t;
    };
};

struct fact_hashtable_node {
    struct fact *f;
    struct fact_hashtable_node *next;
};

struct fact_hashtable {
    int size;
    struct fact_hashtable_node **head;
};

//NOTE:  ASSETS ARE NOT REALLY MUTABLE NETWORK STATE ELEMENTS
// BUT A POINTER TO A GLOBAL ASSET HASHTABLE IS PROBABLY HARMLESS
// AND MAYBE USEFUL. WE'LL SEE....
//NOTE THAT AN UNEXPANDED NETWORK STATE WILL BE A STRUCT CONTAINING A
// PTR TO AN EXPANDED PARENT STATE + A LIST OF OPERATIONS

struct network_state {
    struct asset_hashtable *a_ht;
    struct fact_hashtable *f_ht;
};

unsigned int asset_hash(char *);

struct asset *new_asset(char *, struct identifier_hashtable *);

struct asset_hashtable *make_asset_hashtable(struct asset_hashtable *);

struct asset_hashtable_node *find_asset(char *, struct asset_hashtable *);

struct asset_hashtable *insert_asset(char *, struct asset_hashtable *, struct identifier_hashtable *);

struct asset_hashtable_node *insert_and_get_asset(char *, struct asset_hashtable *, struct identifier_hashtable *);

char *asset_hashtable_to_string(struct asset_hashtable *);

char *fact_to_dot(struct fact_hashtable *, struct identifier_hashtable *);

unsigned int fact_hash(struct fact *);

unsigned int quality_hash(char *, char *);

unsigned int topology_hash(char *, char *, char *);

struct fact *new_quality(char *, char *, char *, struct asset_hashtable *, struct identifier_hashtable *);

struct fact *new_topology(char *, char *, char *, struct asset_hashtable *, struct identifier_hashtable *);

struct fact_hashtable *make_fact_hashtable();

struct fact_hashtable_node *find_quality(char *, char *, struct fact_hashtable *);

struct fact_hashtable_node *find_topology(char *, char *, char *, struct fact_hashtable *);

struct fact_hashtable_node *find_quality_with_hash(char *, char *, struct fact_hashtable *, unsigned int);

struct fact_hashtable_node *find_topology_with_hash(char *, char *, char *, struct fact_hashtable *, unsigned int);

struct fact_hashtable *insert_fact(struct fact *, struct fact_hashtable *, struct identifier_hashtable *);

struct fact *prepare_quality(char *, char *, char *, struct asset_hashtable *, struct fact_hashtable *,
                             struct identifier_hashtable *);

struct fact *prepare_topology(char *, char *, char *, struct asset_hashtable *, struct fact_hashtable *,
                              struct identifier_hashtable *);

char *fact_hashtable_to_string(struct fact_hashtable *);

struct network_state *new_network_state(struct network_state *, struct asset_hashtable *, struct fact_hashtable *);

char *network_state_to_string(struct network_state *);

#endif