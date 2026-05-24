#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

class Graph {
private:
    std::vector<std::vector<int>> adj_;
    int vertices_;
    std::vector<int>  dist_;
    std::vector<int>  parent_;
    std::vector<bool> visited_;
    std::vector<int>  order_;

    void resetArrays();
    void findDiameterPath(int start, std::vector<int>& outPath, int& outDiameter);

public:
    Graph();
    Graph(int n);
    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    ~Graph() = default;

    // Базовые операции
    bool isEmpty() const;
    bool hasVertex(int v) const;
    bool hasEdge(int u, int v) const;
    void addVertex();
    void removeVertex(int v);
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    int getVertexCount() const;
    void print() const;

    // 1. Обход в ширину (BFS)
    void bfs(int start);
    void printBFSTree(int start);

    // 2. Кратчайший путь
    void printShortestPath(int start, int target);

    // 3. Диаметр дерева BFS
    int getDiameter(int start);
    void printDiameterPath(int start);
};

#endif
