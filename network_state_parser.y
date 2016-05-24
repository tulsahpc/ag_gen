/* C declarations */

%{
#include <stdio.h>
#include "identifier.h"
#include "network_state.h"
#include "globals.h"

// struct asset_hashtable *asset_ht;
// struct fact_hashtable *fact_ht;

int yylex();
void yyerror(char const *);
%}

/* Bison declarations */
%union {
	struct network_state *s;
	struct asset_hashtable *a;
	struct fact_hashtable *fht;
	struct fact *f;
	char *i;
}

%token NETWORK MODEL EQ ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY
%token <i> IDENTIFIER

%type <s> root
%type <a> assetlist
%type <fht> factlist
%type <f> fact
%%

root:	NETWORK MODEL EQ ASSETS COLON assetlist FACTS COLON factlist PERIOD	{/*printf("Root\n"); $$ = new_network_state(ns,$6,$9); printf("ns is %p\n",(void *) $$);*/}
;

assetlist:	assetlist IDENTIFIER SEMI	{/*printf("assetlist rule 1: assetlist is %p\n",$1); */ $$ = insert_asset($2,asset_ht,identifier_ht);} 
	|	/* empty */ {/*printf("assetlist empty\n"); $$ = make_asset_hashtable(asset_ht);*/}	
;

factlist:	factlist fact	{/* printf("factlist rule 1: f_ht is %p\n", (void *) $1);*/ $$ = insert_fact($2,fact_ht,identifier_ht);} //add the fact to factlist
	|	/* empty */ {/* printf("factlist empty\n"); $$ = make_fact_hashtable(fact_ht); */}
;

fact:	QUALITY COLON  IDENTIFIER COMMA IDENTIFIER COMMA IDENTIFIER SEMI	{/* printf("quality rule: asset_ht is %p; fact_ht is %p; identifier_ht is %p\n", (void *) asset_ht, (void *) fact_ht, (void *) identifier_ht);*/ $$ = prepare_quality($3,$5,$7,asset_ht,fact_ht,identifier_ht);}
	| TOPOLOGY COLON IDENTIFIER COMMA IDENTIFIER COMMA IDENTIFIER SEMI	{/* printf("topology rule\n");*/ $$ = prepare_topology($3,$5,$7,asset_ht,fact_ht,identifier_ht);}
;
%%

void yyerror(char const *str) {
	fprintf (stderr, "%s\n", str);
}
