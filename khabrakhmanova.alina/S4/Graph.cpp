#include "Graph.h"
#include <queue>
#include <algorithm>
#include <iostream>
#include <climits>

const std::string ERROR_GRAPH_EMPTY = "Error: Graph is empty";
const std::string ERROR_EDGE_NOT_FOUND = "Error: Edge not found between nodes";
const std::string ERROR_INVALID_NODE = "Error: Invalid node index";
const std::string ERROR_NEGATIVE_WEIGHT = "Error: Edge weight cannot be negative";
const std::string ERROR_NODE_ALREADY_EXISTS = "Error: Node already exists in graph";
const std::string ERROR_NO_MST = "Error: Graph is not connected, MST does not exist";
const std::string ERROR_SELF_LOOP = "Error: Self-loops are not allowed";

DisjointSet::DisjointSet(int n) : parent(n), rank(n, 0) {
    if (n < 0) {
        throw std::runtime_error("Invalid size for DisjointSet");
    }
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int x) {
    if (x < 0 || x >= static_cast<int>(parent.size())) {
        throw std::runtime_error("DisjointSet: index out of bounds");
    }
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void DisjointSet::unite(int x, int y) {
    if (x < 0 || x >= static_cast<int>(parent.size()) ||
        y < 0 || y >= static_cast<int>(parent.size())) {
        throw std::runtime_error("DisjointSet: index out of bounds");
    }
    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            ++rank[rootX];
        }
    }
}

bool DisjointSet::isConnected(int x, int y) {
    return find(x) == find(y);
}

Graph::Graph() : vertexCount(0), edgeCount(0) {}

Graph::Graph(const Graph& other)
    : adjacencyList(other.adjacencyList), vertexCount(other.vertexCount), edgeCount(other.edgeCount) {}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        adjacencyList = other.adjacencyList;
        vertexCount = other.vertexCount;
        edgeCount = other.edgeCount;
    }
    return *this;
}

void Graph::validateNode(int node) const {
    if (!hasNode(node)) {
        throw std::runtime_error(ERROR_INVALID_NODE);
    }
}

void Graph::validateNonNegativeWeight(int weight) const {
    if (weight < 0) {
        throw std::runtime_error(ERROR_NEGATIVE_WEIGHT);
    }
}

void Graph::validateNoNegativeWeights() const {
    for (const auto& pair : adjacencyList) {
        for (const auto& edge : pair.second) {
            if (edge.weight < 0) {
                throw std::runtime_error(ERROR_NEGATIVE_WEIGHT);
            }
        }
    }
}

void Graph::validateNoSelfLoop(int from, int to) const {
    if (from == to) {
        throw std::runtime_error(ERROR_SELF_LOOP);
    }
}

void Graph::createEmpty() {
    adjacencyList.clear();
    vertexCount = 0;
    edgeCount = 0;
}

bool Graph::isEmpty() const {
    return vertexCount == 0;
}

bool Graph::hasNode(int node) const {
    return adjacencyList.find(node) != adjacencyList.end();
}

bool Graph::hasEdge(int from, int to) const {
    if (!hasNode(from) || !hasNode(to)) {
        return false;
    }

    const auto& edges = adjacencyList.at(from);
    for (const auto& edge : edges) {
        if (edge.to == to) {
            return true;
        }
    }
    return false;
}

int Graph::getEdgeWeight(int from, int to) const {
    if (!hasNode(from) || !hasNode(to)) {
        throw std::runtime_error(ERROR_INVALID_NODE);
    }

    const auto& edges = adjacencyList.at(from);
    for (const auto& edge : edges) {
        if (edge.to == to) {
            return edge.weight;
        }
    }
    throw std::runtime_error(ERROR_EDGE_NOT_FOUND);
}

void Graph::addNode() {
    int newNode = vertexCount;
    adjacencyList[newNode] = std::vector<Edge>();
    ++vertexCount;
}

void Graph::addNode(int node) {
    if (node < 0) {
        throw std::runtime_error(ERROR_INVALID_NODE);
    }
    if (hasNode(node)) {
        throw std::runtime_error(ERROR_NODE_ALREADY_EXISTS);
    }

    adjacencyList[node] = std::vector<Edge>();
    ++vertexCount;

}

void Graph::removeNode(int node) {
    validateNode(node);

    for (auto& pair : adjacencyList) {
        auto& edges = pair.second;
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [node](const Edge& e) { return e.to == node; }), edges.end());
    }

    edgeCount -= static_cast<int>(adjacencyList[node].size());
    adjacencyList.erase(node);
    --vertexCount;
}

void Graph::addEdge(int from, int to, int weight) {
    validateNode(from);
    validateNode(to);
    validateNonNegativeWeight(weight);
    validateNoSelfLoop(from, to);

    bool edgeExists = false;
    auto& edgesFrom = adjacencyList[from];
    for (auto& edge : edgesFrom) {
        if (edge.to == to) {
            edge.weight = weight;
            edgeExists = true;
            break;
        }
    }

    auto& edgesTo = adjacencyList[to];
    for (auto& edge : edgesTo) {
        if (edge.to == from) {
            edge.weight = weight;
            break;
        }
    }

    if (!edgeExists) {
        adjacencyList[from].push_back(Edge(to, weight));
        adjacencyList[to].push_back(Edge(from, weight));
        ++edgeCount;
    }
}

void Graph::removeEdge(int from, int to) {
    validateNode(from);
    validateNode(to);

    bool edgeRemoved = false;

    auto& edgesFrom = adjacencyList[from];
    auto itFrom = std::remove_if(edgesFrom.begin(), edgesFrom.end(),
        [to](const Edge& e) { return e.to == to; });
    if (itFrom != edgesFrom.end()) {
        edgesFrom.erase(itFrom, edgesFrom.end());
        edgeRemoved = true;
    }

    auto& edgesTo = adjacencyList[to];
    auto itTo = std::remove_if(edgesTo.begin(), edgesTo.end(),
        [from](const Edge& e) { return e.to == from; });
    if (itTo != edgesTo.end()) {
        edgesTo.erase(itTo, edgesTo.end());
    }

    if (!edgeRemoved) {
        throw std::runtime_error(ERROR_EDGE_NOT_FOUND);
    }

    --edgeCount;
}

std::vector<EdgeForKruskal> Graph::kruskalMST() const {
    std::vector<EdgeForKruskal> result;

    if (isEmpty()) {
        throw std::runtime_error(ERROR_GRAPH_EMPTY);
    }

    validateNoNegativeWeights();

    std::vector<EdgeForKruskal> allEdges;
    for (const auto& pair : adjacencyList) {
        int from = pair.first;
        for (const auto& edge : pair.second) {
            if (edge.to > from) {
                allEdges.push_back(EdgeForKruskal(from, edge.to, edge.weight));
            }
        }
    }

    std::sort(allEdges.begin(), allEdges.end(),
        [](const EdgeForKruskal& a, const EdgeForKruskal& b) {
            return a.weight < b.weight;
        });

    DisjointSet ds(vertexCount);

    for (const auto& edge : allEdges) {
        if (!ds.isConnected(edge.from, edge.to)) {
            ds.unite(edge.from, edge.to);
            result.push_back(edge);
        }
    }

    if (vertexCount > 1 && static_cast<int>(result.size()) != vertexCount - 1) {
        throw std::runtime_error(ERROR_NO_MST);
    }

    return result;
}

std::vector<EdgeForKruskal> Graph::primMST(int start) const {
    std::vector<EdgeForKruskal> result;

    if (isEmpty()) {
        throw std::runtime_error(ERROR_GRAPH_EMPTY);
    }

    if (!hasNode(start)) {
        throw std::runtime_error(ERROR_INVALID_NODE);
    }

    validateNoNegativeWeights();

    std::vector<bool> visited(vertexCount, false);
    std::vector<int> minEdge(vertexCount, INT_MAX);
    std::vector<int> parent(vertexCount, -1);

    minEdge[start] = 0;

    for (int count = 0; count < vertexCount; ++count) {
        int u = -1;
        int min = INT_MAX;

        for (int i = 0; i < vertexCount; ++i) {
            if (!visited[i] && minEdge[i] < min) {
                min = minEdge[i];
                u = i;
            }
        }

        if (u == -1) {
            throw std::runtime_error(ERROR_NO_MST);
        }

        visited[u] = true;

        if (parent[u] != -1) {
            result.push_back(EdgeForKruskal(parent[u], u, minEdge[u]));
        }

        if (adjacencyList.find(u) != adjacencyList.end()) {
            for (const auto& edge : adjacencyList.at(u)) {
                int v = edge.to;
                int weight = edge.weight;

                if (!visited[v] && weight < minEdge[v]) {
                    minEdge[v] = weight;
                    parent[v] = u;
                }
            }
        }
    }

    return result;
}

void Graph::printGraph() const {
    if (isEmpty()) {
        std::cout << "Graph is empty" << std::endl;
        return;
    }

    std::cout << "\n=== Graph (" << vertexCount << " vertices, " << edgeCount << " edges) ===" << std::endl;
    for (const auto& pair : adjacencyList) {
        std::cout << "Vertex " << pair.first << ": ";
        const auto& edges = pair.second;

        if (edges.empty()) {
            std::cout << "(no edges)";
        } else {
            for (size_t j = 0; j < edges.size(); ++j) {
                if (j > 0) std::cout << ", ";
                std::cout << "--(" << edges[j].weight << ")--> " << edges[j].to;
            }
        }
        std::cout << std::endl;
    }
}

bool Graph::isConnected() const {
    if (isEmpty()) return true;
    if (vertexCount == 1) return true;

    std::vector<bool> visited(vertexCount, false);
    std::queue<int> q;

    int start = adjacencyList.begin()->first;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (adjacencyList.find(u) != adjacencyList.end()) {
            for (const auto& edge : adjacencyList.at(u)) {
                int v = edge.to;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            return false;
        }
    }

    return true;
}

int Graph::getTotalWeight(const std::vector<EdgeForKruskal>& mst) const {
    int total = 0;
    for (const auto& edge : mst) {
        total += edge.weight;
    }
    return total;
}
