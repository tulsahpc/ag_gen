//
// Created by archlord on 6/5/16.
//

#ifndef NETMODGEN_NETMOD_H
#define NETMODGEN_NETMOD_H

#include <iostream>
#include <stdlib.h>
#include <vector> //Sort of like a linked list

using namespace std;

struct quality {
    int id;
    string type;
    string value;
};

struct topology {
    int asset1;
    int asset2;
    string relationship;
};

int genNetModel();

int selectAsset( vector<string> assetVector );

string addAsset();

int qualityMangager( int id, vector<quality> &qualities );

string fixString( string fixMe );

int topolgyManager( vector<string> assets, vector<topology> &topologies );

void printSql( string networkName, vector<string> assets, vector<quality> qualities, vector<topology> topologies );


#endif //NETMODGEN_NETMOD_H
