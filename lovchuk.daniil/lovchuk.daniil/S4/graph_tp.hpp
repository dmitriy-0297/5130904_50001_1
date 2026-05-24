#ifndef GRAPH_TP_HPP
#define GRAPH_TP_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <functional>

using namespace std;

class Graph {
private:
    vector<vector<int>> adj;
    int vertices;
    vector<int> dist;
    vector<int> parent;
    vector<bool> visited;
    vector<int> order;

    void processQueueRecursive(queue<int>& q);
    void bfsRecursive(queue<int>& q, vector<int>& d);

public:
    Graph();
    void init(int n);
    void addVertex();
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    void clear();
    int countVertices() const;
    bool isBlank() const;

    void executeBFS(int start);
    void printBFSTree(int start);
    void printShortestPath(int u, int v);
    int computeDiameter(int start);
    void printDiameter(int start);
    void printGraph();
};

#endif
