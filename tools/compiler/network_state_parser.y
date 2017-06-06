%{
#include <stdio.h>
#include "identifier.h"
#include "network_state.h"
#include "globals.h"

int nslex();
void nserror(char const *);
%}

%name-prefix="ns"

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

root: NETWORK MODEL EQ ASSETS COLON assetlist FACTS COLON factlist PERIOD {}
;

assetlist: assetlist IDENTIFIER SEMI { $$ = insert_asset($2,asset_ht,identifier_ht); } 
	| {}
;

factlist: factlist fact { $$ = insert_fact($2,fact_ht,identifier_ht); }
	| {}
;

fact: QUALITY COLON IDENTIFIER COMMA IDENTIFIER COMMA IDENTIFIER SEMI { $$ = prepare_quality($3, $5, $7, asset_ht, fact_ht, identifier_ht); }
	| TOPOLOGY COLON IDENTIFIER COMMA IDENTIFIER COMMA IDENTIFIER SEMI { $$ = prepare_topology($3, $5, $7, asset_ht, fact_ht, identifier_ht); }
;

%%

void nserror(char const *str) {
	fprintf (stderr, "%s\n", str);
}
