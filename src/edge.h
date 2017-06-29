//
// Created by Kyle Cook on 6/24/17.
//

#ifndef AG_GEN_EDGE_H
#define AG_GEN_EDGE_H

class Edge {
	int from_node;
	int to_node;

public:
	Edge(int iFrom, int iTo) : from_node(iFrom), to_node(iTo) {}

	bool exists_in_db(void);
	void save(void);
};

#endif //AG_GEN_EDGE_H
