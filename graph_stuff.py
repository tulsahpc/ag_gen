#!/usr/bin/env python

import psycopg2 as pg
import networkx as nx

conn = pg.connect("dbname=ag_gen2 user=capt_redbeard")

with conn.cursor() as curr:
    curr.execute("SELECT from_node,to_node FROM edge;");
    edges = curr.fetchall()

conn.close()

G = nx.DiGraph()
G.add_edges_from(edges)

print("Nodes: " + str(G.order()))
print("Edges: " + str(G.size()))

if(nx.is_directed_acyclic_graph(G)):
    print("G is a DAG.")
else:
    print("G is NOT a DAG.")

print(nx.dag_longest_path_length(G))
