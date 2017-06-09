#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_traits.hpp>
#include <libpq-fe.h>

struct Ve {
    std::string id;
    std::string name;
    std::string network_id;
};
struct Ed {
    std::string option;
};

void graph_db(const char *conninfo) {

        PGconn      *conn;
        PGresult    *res;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Ve, Ed> Graph;
        typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
        typedef boost::graph_traits<Graph>::edge_descriptor Edge;

        Graph g;

        conn = PQconnectdb(conninfo);
        if (PQstatus(conn) != CONNECTION_OK) {
                fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
                PQfinish(conn);
                exit(1);
        }
    
        res = PQexec(conn, "SELECT * FROM asset;");
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
                PQclear(res);
                PQfinish(conn);
                exit(1);
        }
        int rows = PQntuples(res);

        std::unordered_map<std::string, Vertex> vertex_map;
        for (int i=0; i<rows /*Number of Assets*/; i++) {
                Vertex v = boost::add_vertex(g);
                g[v].id = PQgetvalue(res, i, 0); // query for id
                g[v].name = PQgetvalue(res, i, 1);
                g[v].network_id = PQgetvalue(res, i, 2);
                vertex_map[ g[v].id] = v;
        }
        PQclear(res);	

        res = PQexec(conn, "SELECT * FROM topology;");
        rows = PQntuples(res);

        for (int i=0; i<rows /*Number of Topologies*/; i++) {
                Edge e1; bool added;
                std::string from = PQgetvalue(res, i, 0);
                std::string to = PQgetvalue(res, i, 1);
                boost::tie(e1, added) = boost::add_edge(vertex_map[from], vertex_map[to], g);
                g[e1].option = PQgetvalue(res, i, 2);
        }

        std::ofstream gout;
        gout.open("graph.dot");
        boost::dynamic_properties dp;
        dp.property("label", get(&Ve::name, g));
        dp.property("node_id", get(boost::vertex_index, g));
        boost::write_graphviz_dp(gout, g, dp);
}

int main() {
        const char *conninfo;
        conninfo = "postgresql://ruff@localhost/mydb2";
        graph_db(conninfo);
        
        return 0;
}
