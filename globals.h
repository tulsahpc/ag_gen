/*
 * globals.h
 *
 *  Created on: Jan 10, 2016
 *      Author: john-hale
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

extern int num_exploits;
extern struct identifier_hashtable *identifier_ht;
extern struct asset_hashtable *asset_ht;
extern struct fact_hashtable *fact_ht;
extern struct network_state *ns;
extern struct exploit_list_node *exploits;

#endif