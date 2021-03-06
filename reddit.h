#pragma once
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include "graph.h"

namespace reddit {

class Reddit {

  public:
    Reddit() : g_(true, true), g_flipped_(false, true) {};
    ~Reddit();

    /**
     * Parses through a data file to create the graph.
     * @param data_file the string representing the path of the file
     */
    void parseData(const std::string& data_file);

    /**
     * Prints out the graph.
     */
    void printData();

    /**
     * Uses the page rank algorithm to print out a distribution of the nodes in each connected component.
     */
    void pagerank();

    /**
     * Returns the graph created by parseData()
     */
    const Graph& getGraph();

    /**
     * Returns the connected components created by findConnectedComponents(),
     * which is called by pagerank().
     * @return a vector of vectors containing the nodes in each component
     */
    const std::vector<std::vector<Vertex>>& getConnectedComponents();

    /**
     * Returns the pagerank distribution created by pagerank().
     * @return a vector of maps mapping each vertex to its probability
     */
    const std::vector<std::map<Vertex, double>>& getPagerankDistr();

    //FLOYD WARSHALL FUNCTIONS
    /**
     * Prints path taken between two user-inputted subreddits
     * @param start starting vertex
     * @param dest ending vertex
     */
    void printInputPath(const Vertex& start, const Vertex& dest);

    /**
     * Prints the floyd-warshall algorithm of calculating the shortest path between
     * every pair of nodes
     */
    void printFW();

    /**
     * Returns the path created by printInputPath().
     * If called after printFW(), returns the path between the last pair of nodes.
     * @return a vector storing the vertices from start to end
     */
    const vector<Vertex>& getPath();

  private:

    // PAGERANK HELPER METHODS
    /**
     * Splits the graph into its connected components and populates the vector connected_components_ with each
     * component/subgraph, used for the Pagerank algorithm.
     */
    void findConnectedComponents();

    /**
     * Helper method for findConnectedComponents() that uses DFS to traverse through the graph.
     * @param visited_vertex current visited vertex
     * @param visted map storing whether each vertex has been visited
     * @param connected vector storing vertices in one connected component
     */
    void DFS(const Vertex& visited_vertex, std::map<Vertex, bool>& visited, std::vector<Vertex>& connected);

    /**
     * Handles dangling nodes in the connected components by having them point to all other nodes.
     * A dangling node is one with no outgoing edges.
     */
    void handleDanglingNodes();

    // FLOYD-WARSHALL HELPER METHODS
    /**
    * Calculates all the shortest path between every pair of nodes
    */
    void buildShortestPaths();

    /**
     * Helper function for buildShortestPaths() that gets the shortest distance between two nodes
     * @param source starting vertex
     * @param dest ending vertex
     */
    int getshortestDist(const Vertex& source, const Vertex& dest); 

    /**
     * Obtains shortest path between two user-inputted subreddits
     * @param source starting vertex
     * @param dest ending vertex
     */    
    void findPath(const Vertex& source, const Vertex& dest);
    
    vector<Vertex> path_; // stores shortest path from starting vertex to end
    Graph g_;
    Graph g_flipped_; // stores a flipped version of the actual graph in order to easily get vertices pointing TO a certain vertex
    std::vector<std::map<Vertex, double>> pagerank_distr_;
    std::vector<std::vector<Vertex>> connected_components_;

    //FLOYD WARSHALL VARIABLES
    std::map<Vertex, int> vertextoInt;
    int* minDist = NULL;
    int* next = NULL;
    
};


}