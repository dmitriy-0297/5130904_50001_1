#include "bellman_ford.h"
#include <limits>
#include <stdexcept>
#include <functional>

static const std::string ERROR_NEGATIVE_CYCLE = "Negative cycle detected";
static const std::string ERROR_NODE_NOT_FOUND = "Node not found";
static const std::string ERROR_GRAPH_EMPTY = "Graph is empty";

static void relaxNeighborsRecursive(
    const std::vector<std::pair<int, double>>& neighbors,
    size_t index,
    int u,
    std::map<int, double>& distances,
    std::map<int, int>& predecessors,
    bool& any_update) {

    if (index >= neighbors.size()) {
        return;
    }

    int v = neighbors[index].first;
    double weight = neighbors[index].second;

    if (distances[u] + weight < distances[v]) {
        distances[v] = distances[u] + weight;
        predecessors[v] = u;
        any_update = true;
    }

    relaxNeighborsRecursive(neighbors, index + 1, u, distances, predecessors, any_update);
}

static void relaxAllEdgesRecursive(
    std::set<int>::const_iterator nodeIt,
    std::set<int>::const_iterator nodesEnd,
    const DirectedWeightedGraph& graph,
    std::map<int, double>& distances,
    std::map<int, int>& predecessors,
    bool& any_update) {

    if (nodeIt == nodesEnd) {
        return;
    }

    int u = *nodeIt;

    if (distances[u] != std::numeric_limits<double>::infinity()) {
        relaxNeighborsRecursive(graph.getNeighbors(u), 0, u, distances, predecessors, any_update);
    }

    relaxAllEdgesRecursive(std::next(nodeIt), nodesEnd, graph, distances, predecessors, any_update);
}

static void bellmanFordIterationsRecursive(
    size_t iteration,
    size_t n,
    const DirectedWeightedGraph& graph,
    std::map<int, double>& distances,
    std::map<int, int>& predecessors) {

    if (iteration >= n - 1) {
        return;
    }

    bool any_update = false;
    relaxAllEdgesRecursive(graph.getNodes().begin(), graph.getNodes().end(),
                          graph, distances, predecessors, any_update);

    if (!any_update) {
        return;
    }

    bellmanFordIterationsRecursive(iteration + 1, n, graph,
                                  distances, predecessors);
}

static void checkNegativeNeighborsRecursive(
    const std::vector<std::pair<int, double>>& neighbors,
    size_t index,
    int u,
    const std::map<int, double>& distances) {

    if (index >= neighbors.size()) {
        return;
    }

    int v = neighbors[index].first;
    double weight = neighbors[index].second;

    if (distances.at(u) + weight < distances.at(v)) {
        throw std::runtime_error(ERROR_NEGATIVE_CYCLE);
    }

    checkNegativeNeighborsRecursive(neighbors, index + 1, u, distances);
}

static void checkNegativeCycleRecursive(
    std::set<int>::const_iterator nodeIt,
    std::set<int>::const_iterator nodesEnd,
    const DirectedWeightedGraph& graph,
    const std::map<int, double>& distances) {

    if (nodeIt == nodesEnd) {
        return;
    }

    int u = *nodeIt;

    if (distances.at(u) != std::numeric_limits<double>::infinity()) {
        checkNegativeNeighborsRecursive(graph.getNeighbors(u), 0, u, distances);
    }

    checkNegativeCycleRecursive(std::next(nodeIt), nodesEnd, graph, distances);
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

std::pair<std::map<int, double>, std::map<int, int>> bellmanFordShortestPath(
    const DirectedWeightedGraph& graph, int source) {

    if (!graph.hasNode(source)) {
        throw std::invalid_argument(ERROR_NODE_NOT_FOUND);
    }

    if (graph.isEmpty()) {
        throw std::runtime_error(ERROR_GRAPH_EMPTY);
    }

    std::map<int, double> distances;
    std::map<int, int> predecessors;

    initDistancesRecursive(graph.getNodes().begin(), graph.getNodes().end(),
                          distances, predecessors, source);
    distances[source] = 0.0;

    size_t n = graph.getNodeCount();
    bellmanFordIterationsRecursive(0, n, graph, distances, predecessors);

    checkNegativeCycleRecursive(graph.getNodes().begin(), graph.getNodes().end(),
                               graph, distances);

    return std::make_pair(distances, predecessors);
}
