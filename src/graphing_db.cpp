#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_traits.hpp>
#include <libpq-fe.h>
#include <cstdlib> // for std::system()

#include "graphing_db.h"

struct Factbase_Node {
        std::string factbase_id;
        std::string hash;
};
struct Factbase_Edge {
};
struct Asset_Node {
        std::string id;
        std::string name;
        std::string network_id;
};
struct Topology_Edge {
        std::string option;
};

void Graph::checkSelect(PGconn *conn, PGresult *res) {
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
                PQclear(res);
                PQfinish(conn);
                exit(1);
        }
}

void Graph::graph_db(PGconn *conn) {

        PGresult    *res;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Factbase_Node, Factbase_Edge> Attack_Graph;
        typedef boost::graph_traits<Attack_Graph>::vertex_descriptor Attack_Vertex;
        typedef boost::graph_traits<Attack_Graph>::edge_descriptor Attack_Edge;
        Attack_Graph ag;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Asset_Node, Topology_Edge> Net_Graph;
        typedef boost::graph_traits<Net_Graph>::vertex_descriptor Net_Vertex;
        typedef boost::graph_traits<Net_Graph>::edge_descriptor Net_Edge;
        Net_Graph ng;
   
        res = PQexec(conn, "SELECT * FROM factbase;");
        checkSelect(conn, res);
        int rows = PQntuples(res);
        std::unordered_map<std::string, Attack_Vertex> att_vertex_map;
        for (int i=0; i<rows; i++) {
                Attack_Vertex v = boost::add_vertex(ag);
                ag[v].factbase_id = PQgetvalue(res, i, 0);
                ag[v].hash = PQgetvalue(res, i, 1);
                att_vertex_map[ ag[v].factbase_id] = v;
        }
        PQclear(res);	

        res = PQexec(conn, "SELECT * FROM edge;");
        checkSelect(conn, res);
        rows = PQntuples(res);
        for (int i=0; i<rows; i++) {
                Attack_Edge edge; bool added;
                std::string from = PQgetvalue(res, i, 0);
                std::string to = PQgetvalue(res, i, 1);
                boost::tie(edge, added) = boost::add_edge(att_vertex_map[from], att_vertex_map[to], ag);
        }
        PQclear(res);
        
        res = PQexec(conn, "SELECT * FROM asset;");
        checkSelect(conn, res);
        rows = PQntuples(res);
        std::unordered_map<std::string, Net_Vertex> net_vertex_map;
        for (int i=0; i<rows; i++) {
                Net_Vertex v = boost::add_vertex(ng);
                ng[v].id = PQgetvalue(res, i, 0);
                ng[v].name = PQgetvalue(res, i, 1);
                ng[v].network_id = PQgetvalue(res, i, 2);
                net_vertex_map[ ng[v].id] = v;
        }
        PQclear(res);	

        res = PQexec(conn, "SELECT * FROM topology;");
        checkSelect(conn, res);
        rows = PQntuples(res);
        for (int i=0; i<rows; i++) {
                Net_Edge edge; bool added;
                std::string from = PQgetvalue(res, i, 0);
                std::string to = PQgetvalue(res, i, 1);
                boost::tie(edge, added) = boost::add_edge(net_vertex_map[from], net_vertex_map[to], ng);
                ng[edge].option = PQgetvalue(res, i, 2);
        }
        PQclear(res);

        std::ofstream gout;
        gout.open("att_graph.circo");
        boost::dynamic_properties dp;
        dp.property("label", get(&Factbase_Node::factbase_id, ag));
        dp.property("node_id", get(boost::vertex_index, ag));
        boost::write_graphviz_dp(gout, ag, dp);
        gout.close();
        //XXX: The use of std::system is horrible!
        std::system("circo -Tpdf att_graph.circo -o graph_a.pdf");
        
        gout.open("net_graph.circo");
        boost::dynamic_properties dp2;
        dp2.property("label", get(&Asset_Node::name, ng));
        dp2.property("node_id", get(boost::vertex_index, ng));
        boost::write_graphviz_dp(gout, ng, dp2);
        //XXX: The use of std::system is horrible!
        std::system("circo -Tpdf net_graph.circo -o graph_n.pdf");
}
