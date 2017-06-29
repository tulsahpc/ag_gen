//
// Created by Kyle Cook on 6/24/17.
//

#ifndef AG_GEN_EDGE_H
#define AG_GEN_EDGE_H

#include "exploit.h"
#include "assetgroup.h"

class Edge {
	int id;
	int from_node;
	int to_node;
	Exploit exploit;
	AssetGroup assetGroup;

public:
	Edge(int, int, Exploit&, AssetGroup&);

	void save(void);

	int new_id(void);
	int get_id(void);
	bool exists_in_db(void);
};

#endif //AG_GEN_EDGE_H
