/*
 * ag_generator_utils.h
 *
 *  Created on: Jan 16, 2016
 *      Author: john-hale
 */

#ifndef AG_GENERATOR_UTILS_H_
#define AG_GENERATOR_UTILS_H_

struct bound_param {
    struct param *p;
    struct asset *a;
};

struct bound_params_node {
    struct bound_param *bp;
    struct bound_params_node *next;
    struct bound_params_node *prev;
};

struct bound_params_list_node {
    struct bound_params_node *bp_n;
    struct bound_params_list_node *next;
    struct bound_params_list_node *prev;
};

struct bound_param_lists *exploit_asset_bindings(struct asset_hashtable *, struct exploit *);

#endif