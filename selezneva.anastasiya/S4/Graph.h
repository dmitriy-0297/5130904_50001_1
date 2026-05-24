#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <forward_list>
#include <string>
#include <stdexcept>
#include <iostream>


class Graph {
private:
    struct Edge {
        int vertex = 0;
        int weight = 1;

        Edge() = default;
        Edge(int v, int w = 1) : vertex(v), weight(w) {}

        bool operator==(const Edge& other) const { return vertex == other.vertex; }
        friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
            std::ostream::sentry sentry(os);
            if (sentry) {
                os << " -> " << edge.vertex;
                if (edge.weight != 1) os << "(w:" << edge.weight << ")";
            }
            return os;
        };
    };
    enum Color {
        WHITE = 0,
        GRAY = 1,
        BLACK = 2
    };

    std::vector<std::forward_list<Edge>> adjacencyList_;
    int  vertexCount_ = 0;
    int  edgeCount_ = 0;
    bool oriented_ = true;

    void dfsVisit(int vertex, std::vector<int>& colors, std::vector<int>& result);
    void topoVisit(int vertex, std::vector<int>& colors, std::vector<int>& stack);
    void validateVertex(int vertex) const;
public:
    explicit Graph(bool oriented = true);
    explicit Graph(int vertexCount, bool oriented = true);

    Graph(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(const Graph&) = default;
    Graph& operator=(Graph&&) = default;
    ~Graph() = default;

    bool isEmpty() const;
    bool hasVertex(int vertex) const;
    bool hasEdge(int u, int v) const;
    int  getEdgeWeight(int u, int v) const;

    int  addVertex();
    void removeVertex(int vertex);
    void addEdge(int u, int v, int weight = 1);
    int  removeEdge(int u, int v);

    int  getVertexCount() const;
    int  getEdgeCount()   const;
    bool isOriented()     const;

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

    std::vector<int> dfs(int startVertex);
    std::vector<int> topologicalSort();
};

#endif
