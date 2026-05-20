#include "wave_algorithm.h"
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <functional>

static const std::string ERROR_NODE_NOT_FOUND = "One or both nodes not found";
static const std::string ERROR_GRAPH_EMPTY = "Graph is empty";
static const std::string ERROR_PATH_RECONSTRUCTION = "Node not found";

static void processNeighborsRecursive(
    const std::vector<std::pair<int, double>>& neighbors,
    size_t index,
    std::queue<int>& q,
    std::map<int, bool>& visited,
    std::map<int, int>& predecessors,
    int current) {

    if (index >= neighbors.size()) {
        return;
    }

    int neighbor = neighbors[index].first;

    if (!visited[neighbor]) {
        visited[neighbor] = true;
        predecessors[neighbor] = current;
        q.push(neighbor);
    }

    processNeighborsRecursive(neighbors, index + 1, q, visited, predecessors, current);
}

static void bfsRecursive(
    std::queue<int>& q,
    std::map<int, bool>& visited,
    std::map<int, int>& predecessors,
    const DirectedWeightedGraph& graph,
    int target,
    bool& found) {

    if (q.empty() || found) {
        return;
    }

    int current = q.front();
    q.pop();

    if (current == target) {
        found = true;
        return;
    }

    processNeighborsRecursive(graph.getNeighbors(current), 0, q, visited, predecessors, current);

    bfsRecursive(q, visited, predecessors, graph, target, found);
}

static void initVisitedRecursive(
    std::set<int>::const_iterator it,
    std::set<int>::const_iterator end,
    std::map<int, bool>& visited,
    std::map<int, int>& predecessors) {

    if (it == end) {
        return;
    }

    int node = *it;
    visited[node] = false;
    predecessors[node] = -1;

    initVisitedRecursive(std::next(it), end, visited, predecessors);
}

std::pair<std::map<int, int>, bool> waveShortestPath(
    const DirectedWeightedGraph& graph, int source, int target) {

    if (!graph.hasNode(source) || !graph.hasNode(target)) {
        throw std::invalid_argument(ERROR_NODE_NOT_FOUND);
    }

    if (graph.isEmpty()) {
        throw std::runtime_error(ERROR_GRAPH_EMPTY);
    }

    std::map<int, int> predecessors;
    std::map<int, bool> visited;
    std::queue<int> q;

    initVisitedRecursive(graph.getNodes().begin(), graph.getNodes().end(),
                        visited, predecessors);

    visited[source] = true;
    q.push(source);

    bool found = false;
    bfsRecursive(q, visited, predecessors, graph, target, found);

    return std::make_pair(predecessors, found);
}

static void reconstructPathRecursive(
    int at,
    const std::map<int, int>& predecessors,
    std::vector<int>& path) {

    if (at == -1) {
        return;
    }

    std::map<int, int>::const_iterator it = predecessors.find(at);
    if (it != predecessors.end()) {
        reconstructPathRecursive(it->second, predecessors, path);
    }
    path.push_back(at);
}

std::vector<int> reconstructPath(
    const DirectedWeightedGraph& graph,
    const std::map<int, int>& predecessors,
    int target) {

    std::vector<int> path;

    if (!graph.hasNode(target)) {
        throw std::invalid_argument(ERROR_PATH_RECONSTRUCTION);
    }

    std::map<int, int>::const_iterator it = predecessors.find(target);
    if (it == predecessors.end() || it->second == -1) {
        return path;
    }

    reconstructPathRecursive(target, predecessors, path);
    return path;
}
