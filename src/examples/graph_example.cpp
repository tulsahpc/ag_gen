#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <string>

struct Ve {
    std::string name;
};
struct Ed {
    int cost;
};

int main() {

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                  Ve, Ed>
        Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;

    Graph g;
    Vertex a = boost::add_vertex(g);
    Vertex b = boost::add_vertex(g);
    Vertex c = boost::add_vertex(g);

    Edge e1, e2;
    bool added;
    boost::tie(e1, added) = boost::add_edge(a, b, g);
    boost::tie(e2, added) = boost::add_edge(a, c, g);

    g[a].name = "First";
    g[b].name = "Second";
    g[c].name = "Last";

    std::ofstream gout;
    gout.open("test1.dot");
    boost::dynamic_properties dp;
    dp.property("label", get(&Ve::name, g));
    dp.property("node_id", get(boost::vertex_index, g));
    boost::write_graphviz_dp(gout, g, dp);
}
