#!/usr/bin/env python

import psycopg2 as pg
import networkx as nx

conn = pg.connect("dbname=ag_gen2 user=capt_redbeard")
curr = conn.cursor()

curr.execute("SELECT id FROM factbase;");
nodes = curr.fetchall()

curr.execute("SELECT from_node,to_node FROM edge;");
edges = curr.fetchall()

curr.close()
conn.close()

G = nx.DiGraph()
G.add_nodes_from(nodes)
# G.add_edges_from(edges)

print("Nodes: " + str(G.order()))
# print("Edges: " + str(G.size()))

print(nx.dag_longest_path_length(G))
