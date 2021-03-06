#include "../cs225/catch/catch.hpp"
#include "../reddit.h"
#include <math.h>

using namespace reddit;

TEST_CASE("Small data is correctly parsed into a graph", "[parsedata]") {
    Reddit r;
    r.parseData("data/small_data.tsv");
    Graph g = r.getGraph();
    REQUIRE(g.vertexExists("leagueoflegends"));
    REQUIRE(g.vertexExists("teamredditteams"));
    REQUIRE(g.edgeExists("leagueoflegends", "teamredditteams"));

    REQUIRE(g.vertexExists("theredlion"));
    REQUIRE(g.vertexExists("soccer"));
    REQUIRE(g.edgeExists("theredlion", "soccer"));

    REQUIRE(g.vertexExists("inlandempire"));
    REQUIRE(g.vertexExists("bikela"));
    REQUIRE(g.edgeExists("inlandempire", "bikela"));

    REQUIRE(g.vertexExists("nfl"));
    REQUIRE(g.vertexExists("cfb"));
    REQUIRE(g.edgeExists("nfl", "cfb"));

    REQUIRE(g.vertexExists("playmygame"));
    REQUIRE(g.vertexExists("gamedev"));
    REQUIRE(g.edgeExists("playmygame", "gamedev"));

    REQUIRE(g.vertexExists("dogemarket"));
    REQUIRE(g.vertexExists("dogecoin"));
    REQUIRE(g.edgeExists("dogemarket", "dogecoin"));
}

TEST_CASE("Connected components are divided correctly", "[pagerank]") {
    Reddit r;
    r.parseData("data/mult_connected.tsv");
    r.pagerank();
    std::vector<std::vector<Vertex>> connected_comp = r.getConnectedComponents();
    std::vector<std::vector<Vertex>> actual_comp = {
        {"leagueoflegends",	"teamredditteams", "inlandempire"},
        {"playmygame", "gamedev", "dogemarket", "dogecoin", "nfl", "cfb", "fakereddit"}
    };

    for (unsigned int i = 0; i < connected_comp.size(); i++) {
        std::sort(connected_comp[i].begin(), connected_comp[i].end());
        std::sort(actual_comp[i].begin(), actual_comp[i].end());
    }

    std::sort(connected_comp.begin(), connected_comp.end());
    std::sort(actual_comp.begin(), actual_comp.end());

    REQUIRE(connected_comp == actual_comp);
}

TEST_CASE("Dangling nodes are properly handled", "[pagerank]") {
    Reddit r;
    r.parseData("data/mult_connected.tsv");
    r.pagerank();
    Graph g = r.getGraph();
    std::vector<Vertex> adj = g.getAdjacent("fakereddit");
    std::vector<Vertex> all_vertices = {"playmygame", "gamedev", "dogemarket", "dogecoin", "nfl", "cfb", "fakereddit"};
    std::sort(adj.begin(), adj.end());
    std::sort(all_vertices.begin(), all_vertices.end());

    REQUIRE(adj == all_vertices);
}

TEST_CASE("Pagerank algorithm is correct on one graph", "[pagerank]") {
    double tolerance = 0.00001;
    Reddit r;
    r.parseData("data/one_connected.tsv");
    r.pagerank();
    std::map<Vertex, double> actual = {{"inlandempire", 0.475}, {"leagueoflegends", 0.333333}, {"teamredditteams", 0.191667}};
    std::map<Vertex, double> pagerank = r.getPagerankDistr()[0]; // the distribution only has 1 map in the vector

    double total = 0.0;
    for (auto& vertex : pagerank) {
        REQUIRE(fabs(actual[vertex.first] - vertex.second) <= tolerance);
        total += vertex.second;
    }
    REQUIRE(total - 1.0 < 0.00001);
}

TEST_CASE("Pagerank algorithm is correct on graph with dangling node", "[pagerank]") {
    double tolerance = 0.0001;
    Reddit r;
    r.parseData("data/dangling.tsv");
    r.pagerank();
    std::map<Vertex, double> actual = {
        {"cfb", 0.09948},
        {"dogecoin", 0.2209},
        {"dogemarket", 0.2209},
        {"fakereddit", 0.07925},
        {"gamedev", 0.1602},
        {"nfl", 0.07925},
        {"playmygame", 0.13997}
    };

    std::map<Vertex, double> pagerank = r.getPagerankDistr()[0]; // the distribution only has 1 map in the vector
    double total = 0.0;
    for (auto& vertex : pagerank) {
        REQUIRE(fabs(actual[vertex.first] - vertex.second) <= tolerance);
        total += vertex.second;
    }
    REQUIRE(total - 1.0 < 0.00001);
}

TEST_CASE("Pagerank algorithm works on multiple connected components", "[pagerank]") {
        double tolerance = 0.0001;
    Reddit r;
    r.parseData("data/mult_connected.tsv");
    r.pagerank();
    std::vector<std::map<Vertex, double>> actual = {
        {
            {"cfb", 0.09948},
            {"dogecoin", 0.2209},
            {"dogemarket", 0.2209},
            {"fakereddit", 0.07925},
            {"gamedev", 0.1602},
            {"nfl", 0.07925},
            {"playmygame", 0.13997}
        },
        {
            {"inlandempire", 0.475}, 
            {"leagueoflegends", 0.333333}, 
            {"teamredditteams", 0.191667}
        }
    };

    std::vector<std::map<Vertex, double>> pagerank = r.getPagerankDistr(); // the distribution only has 1 map in the vector
    for (unsigned int i = 0; i < pagerank.size(); i++) {
        double total = 0.0;
        for (auto& vertex : pagerank[i]) {
            REQUIRE(fabs(actual[i][vertex.first] - vertex.second) <= tolerance);
            total += vertex.second;
        }
        REQUIRE(total - 1.0 < 0.00001);
    }
}

TEST_CASE("Floyd Warshall gets the shortest path 1", "[fw]") {
    Reddit r;
    r.parseData("data/FW_test1.tsv");
    r.printInputPath("A", "D");
    std::vector<Vertex> actual = {"B", "C", "D"};
    REQUIRE(r.getPath() == actual);
}

TEST_CASE("Floyd Warshall gets the shortest path 2", "[fw]") {
    Reddit r;
    r.parseData("data/FW_test2.tsv");
    r.printInputPath("A", "D");
    std::vector<Vertex> actual = {"D"};
    REQUIRE(r.getPath() == actual);
}

TEST_CASE("Floyd Warshall gets the shortest path 3", "[fw]") {
    Reddit r;
    r.parseData("data/FW_test3.tsv");
    r.printInputPath("E", "G");
    std::vector<Vertex> actual = {"C", "F", "G"};
    REQUIRE(r.getPath() == actual);
}
