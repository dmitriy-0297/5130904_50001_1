#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>

class DirectedWeightedGraph {
public:
    DirectedWeightedGraph();
    ~DirectedWeightedGraph();

    DirectedWeightedGraph(const DirectedWeightedGraph&) = delete;
    DirectedWeightedGraph& operator=(const DirectedWeightedGraph&) = delete;

    DirectedWeightedGraph(DirectedWeightedGraph&& other) noexcept;
    DirectedWeightedGraph& operator=(DirectedWeightedGraph&& other) noexcept;

    bool isEmpty() const;
    bool hasNode(int node) const;
    bool hasEdge(int from, int to) const;
    void addNode(int node);
    void removeNode(int node);
    void addEdge(int from, int to, double weight);
    double removeEdge(int from, int to);
    double getEdgeWeight(int from, int to) const;
    std::set<int> getNodes() const;
    std::vector<std::pair<int, double>> getNeighbors(int node) const;
    size_t getNodeCount() const;
    size_t getEdgeCount() const;
    void clear();
    void print() const;

private:
    std::map<int, std::vector<std::pair<int, double>>> adjacency_list_;
    std::set<int> nodes_;

    static const std::string ERROR_NODE_NOT_FOUND;
    static const std::string ERROR_NODE_ALREADY_EXISTS;
    static const std::string ERROR_BOTH_NODES_NOT_FOUND;
    static const std::string ERROR_EDGE_NOT_FOUND;
    static const std::string ERROR_SELF_LOOP;
    static const std::string ERROR_EMPTY_GRAPH;

    bool nodeExists(int node) const;

    void removeNodeFromAllEdgesRecursive(typename std::map<int, std::vector<std::pair<int, double>>>::iterator it, int node);
    void printNeighborsRecursive(const std::vector<std::pair<int, double>>& neighbors, size_t index) const;
    void printGraphRecursive(std::set<int>::const_iterator it, const std::set<int>& nodes) const;
    void clearRecursive(typename std::map<int, std::vector<std::pair<int, double>>>::iterator it);
};

#endif
