#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <string>
#include <stdexcept>

extern const std::string ERROR_GRAPH_EMPTY;
extern const std::string ERROR_EDGE_NOT_FOUND;
extern const std::string ERROR_INVALID_NODE;
extern const std::string ERROR_NEGATIVE_WEIGHT;
extern const std::string ERROR_NODE_ALREADY_EXISTS;
extern const std::string ERROR_NO_MST;
extern const std::string ERROR_SELF_LOOP;

struct Edge {
    int to;
    int weight;

    Edge() : to(-1), weight(0) {}
    Edge(int t, int w) : to(t), weight(w) {}
};

struct EdgeForKruskal {
    int from;
    int to;
    int weight;

    EdgeForKruskal() : from(-1), to(-1), weight(0) {}
    EdgeForKruskal(int f, int t, int w) : from(f), to(t), weight(w) {}
};

class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DisjointSet(int n);

    int find(int x);
    void unite(int x, int y);
    bool isConnected(int x, int y);
};

class Graph {
private:
    std::map<int, std::vector<Edge>> adjacencyList;
    int vertexCount;
    int edgeCount;

    void validateNode(int node) const;
    void validateNonNegativeWeight(int weight) const;
    void validateNoNegativeWeights() const;
    void validateNoSelfLoop(int from, int to) const;

public:
    Graph();
    ~Graph() = default;

    Graph(const Graph& other);
    Graph& operator=(const Graph& other);

    void createEmpty();
    bool isEmpty() const;
    bool hasNode(int node) const;
    bool hasEdge(int from, int to) const;
    int getEdgeWeight(int from, int to) const;
    void addNode();
    void addNode(int node);
    void removeNode(int node);
    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);

    int getVertexCount() const { return vertexCount; }
    int getEdgeCount() const { return edgeCount; }
    const std::map<int, std::vector<Edge>>& getAdjacencyList() const { return adjacencyList; }

    std::vector<EdgeForKruskal> kruskalMST() const;
    std::vector<EdgeForKruskal> primMST(int start = 0) const;

    void printGraph() const;
    bool isConnected() const;
    int getTotalWeight(const std::vector<EdgeForKruskal>& mst) const;
};

#endif
