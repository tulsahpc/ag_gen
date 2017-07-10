#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_traits.hpp>
#include <libpq-fe.h>
#include <cstdlib> // for std::system()

#include "util_db.h"
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

void Graph::graph_db() {

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Factbase_Node, Factbase_Edge> Attack_Graph;
        typedef boost::graph_traits<Attack_Graph>::vertex_descriptor Attack_Vertex;
        typedef boost::graph_traits<Attack_Graph>::edge_descriptor Attack_Edge;
        Attack_Graph ag;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Asset_Node, Topology_Edge> Net_Graph;
        typedef boost::graph_traits<Net_Graph>::vertex_descriptor Net_Vertex;
        typedef boost::graph_traits<Net_Graph>::edge_descriptor Net_Edge;
        Net_Graph ng;
   
        std::vector<DB::Row> rows = db->exec("SELECT * FROM factbase;");
        std::unordered_map<std::string, Attack_Vertex> att_vertex_map;
        for (int i=0; i<rows.size(); i++) {
                Attack_Vertex v = boost::add_vertex(ag);
                ag[v].factbase_id = rows[i][0];
                ag[v].hash = rows[i][1];
                att_vertex_map[ ag[v].factbase_id] = v;
        }	

        rows = db->exec("SELECT * FROM edge;");
        for (int i=0; i<rows.size(); i++) {
                Attack_Edge edge; bool added;
                std::string from = rows[i][0];
                std::string to = rows[i][1];
                boost::tie(edge, added) = boost::add_edge(att_vertex_map[from], att_vertex_map[to], ag);
        }
        
        rows = db->exec("SELECT * FROM asset;");
        std::unordered_map<std::string, Net_Vertex> net_vertex_map;
        for (int i=0; i<rows.size(); i++) {
                Net_Vertex v = boost::add_vertex(ng);
                ng[v].id = rows[i][0];
                ng[v].name = rows[i][1];
                ng[v].network_id = rows[i][2];
                net_vertex_map[ ng[v].id] = v;
        }

        rows = db->exec("SELECT * FROM topology;");
        for (int i=0; i<rows.size(); i++) {
                Net_Edge edge; bool added;
                std::string from = rows[i][0];
                std::string to = rows[i][1];
                boost::tie(edge, added) = boost::add_edge(net_vertex_map[from], net_vertex_map[to], ng);
                ng[edge].option = rows[i][2];
        }

        std::ofstream gout;
        gout.open("att_graph.circo");
        boost::dynamic_properties dp;
        dp.property("label", get(&Factbase_Node::factbase_id, ag));
        dp.property("node_id", get(boost::vertex_index, ag));
        boost::write_graphviz_dp(gout, ag, dp);
        gout.close();
        std::string att_str;
        for (int i=0; i<1000; i++) {
                att_str = "att_graph";
                att_str = att_str + std::to_string(i) + ".pdf";
                const char* att_chars = att_str.c_str();
                if (std::ifstream( att_chars ))
                {
                     std::cout << "File " << att_str << " already exists" << std::endl;
                }
                else {break;}
        }
        att_str = "circo -Tpdf att_graph.circo -o " + att_str;
        //XXX: The use of std::system is horrible!
        std::system(att_str.c_str());
        std::system("rm att_graph.circo");
        
        gout.open("net_graph.circo");
        boost::dynamic_properties dp2;
        dp2.property("label", get(&Asset_Node::name, ng));
        dp2.property("node_id", get(boost::vertex_index, ng));
        boost::write_graphviz_dp(gout, ng, dp2);
        std::string net_str;
        for (int i=0; i<1000; i++) {
                net_str = "net_graph";
                net_str = net_str + std::to_string(i) + ".pdf";
                if (std::ifstream( net_str.c_str() ))
                {
                     std::cout << "File " << net_str << " already exists" << std::endl;
                }
                else {break;}
        }
        net_str = "circo -Tpdf net_graph.circo -o " + net_str;
        //XXX: The use of std::system is horrible!
        std::system(net_str.c_str());
        std::system("rm net_graph.circo");
}
