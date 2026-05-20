#include "dijkstra.h"
#include <queue>
#include <limits>
#include <stdexcept>
#include <functional>

static const std::string ERROR_NEGATIVE_WEIGHT = "Dijkstra does not support negative weights";
static const std::string ERROR_NODE_NOT_FOUND = "Node not found";
static const std::string ERROR_GRAPH_EMPTY = "Graph is empty";

static void relaxEdgesRecursive(
    const std::vector<std::pair<int, double>>& neighbors,
    size_t index,
    const std::map<int, double>& distances,
    std::map<int, double>& newDistances,
    std::map<int, int>& predecessors,
    std::priority_queue<std::pair<double, int>,
                       std::vector<std::pair<double, int>>,
                       std::greater<std::pair<double, int>>>& pq,
    int u) {
    
    if (index >= neighbors.size()) {
        return;
    }
    
    const auto& [v, weight] = neighbors[index];
    
    if (weight < 0) {
        throw std::runtime_error(ERROR_NEGATIVE_WEIGHT);
    }
    
    if (distances.at(u) + weight < newDistances[v]) {
        newDistances[v] = distances.at(u) + weight;
        predecessors[v] = u;
        pq.push({newDistances[v], v});
    }
    
    relaxEdgesRecursive(neighbors, index + 1, distances, newDistances,
                       predecessors, pq, u);
}

static void processQueueRecursive(
    std::priority_queue<std::pair<double, int>,
                       std::vector<std::pair<double, int>>,
                       std::greater<std::pair<double, int>>>& pq,
    std::map<int, double>& distances,
    std::map<int, int>& predecessors,
    const DirectedWeightedGraph& graph) {
    
    if (pq.empty()) {
        return;
    }
    
    auto [dist_u, u] = pq.top();
    pq.pop();
    
    if (dist_u > distances[u]) {
        processQueueRecursive(pq, distances, predecessors, graph);
        return;
    }
    
    relaxEdgesRecursive(graph.getNeighbors(u), 0, distances, distances,
                       predecessors, pq, u);
    
    processQueueRecursive(pq, distances, predecessors, graph);
}

static void initDistancesRecursive(
    std::set<int>::const_iterator it,
    std::set<int>::const_iterator end,
    std::map<int, double>& distances,
    std::map<int, int>& predecessors,
    int source) {
    
    if (it == end) {
        return;
    }
    
    int node = *it;
    distances[node] = std::numeric_limits<double>::infinity();
    predecessors[node] = -1;
    
    initDistancesRecursive(std::next(it), end, distances, predecessors, source);
}

std::pair<std::map<int, double>, std::map<int, int>> dijkstraShortestPath(
    const DirectedWeightedGraph& graph, int source) {
    
    if (!graph.hasNode(source)) {
        throw std::invalid_argument(ERROR_NODE_NOT_FOUND);
    }
    
    if (graph.isEmpty()) {
        throw std::runtime_error(ERROR_GRAPH_EMPTY);
    }
    
    std::map<int, double> distances;
    std::map<int, int> predecessors;
    std::priority_queue<std::pair<double, int>,
                       std::vector<std::pair<double, int>>,
                       std::greater<std::pair<double, int>>> pq;
    
    initDistancesRecursive(graph.getNodes().begin(), graph.getNodes().end(),
                          distances, predecessors, source);
    
    distances[source] = 0.0;
    pq.push({0.0, source});
    
    processQueueRecursive(pq, distances, predecessors, graph);
    
    return {distances, predecessors};
}
