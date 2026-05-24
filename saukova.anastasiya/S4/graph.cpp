#include "graph.h"
#include <algorithm>

const std::string DirectedWeightedGraph::ERROR_NODE_NOT_FOUND = "Node not found";
const std::string DirectedWeightedGraph::ERROR_NODE_ALREADY_EXISTS = "Node already exists";
const std::string DirectedWeightedGraph::ERROR_BOTH_NODES_NOT_FOUND = "One or both nodes not found";
const std::string DirectedWeightedGraph::ERROR_EDGE_NOT_FOUND = "Edge not found";
const std::string DirectedWeightedGraph::ERROR_SELF_LOOP = "Self-loops not allowed";
const std::string DirectedWeightedGraph::ERROR_EMPTY_GRAPH = "Graph is empty";

DirectedWeightedGraph::DirectedWeightedGraph() {}

DirectedWeightedGraph::~DirectedWeightedGraph() {
    clear();
}

void DirectedWeightedGraph::clearRecursive(
    std::map<int, std::vector<std::pair<int, double>>>::iterator it) {
    if (it == adjacency_list_.end()) {
        return;
    }

    it->second.clear();
    it->second.shrink_to_fit();

    clearRecursive(std::next(it));
}

void DirectedWeightedGraph::clear() {
    clearRecursive(adjacency_list_.begin());
    adjacency_list_.clear();
    nodes_.clear();
}

DirectedWeightedGraph::DirectedWeightedGraph(DirectedWeightedGraph&& other) noexcept
    : adjacency_list_(std::move(other.adjacency_list_))
    , nodes_(std::move(other.nodes_)) {
    other.nodes_.clear();
}

DirectedWeightedGraph& DirectedWeightedGraph::operator=(DirectedWeightedGraph&& other) noexcept {
    if (this != &other) {
        clear();
        adjacency_list_ = std::move(other.adjacency_list_);
        nodes_ = std::move(other.nodes_);
        other.nodes_.clear();
    }
    return *this;
}

bool DirectedWeightedGraph::nodeExists(int node) const {
    return nodes_.find(node) != nodes_.end();
}

bool DirectedWeightedGraph::isEmpty() const {
    return nodes_.empty();
}

bool DirectedWeightedGraph::hasNode(int node) const {
    return nodeExists(node);
}

bool DirectedWeightedGraph::hasEdge(int from, int to) const {
    if (!nodeExists(from) || !nodeExists(to)) {
        throw std::invalid_argument(ERROR_BOTH_NODES_NOT_FOUND);
    }

    const auto& neighbors = adjacency_list_.at(from);
    auto it = std::find_if(neighbors.begin(), neighbors.end(),
                           [to](const std::pair<int, double>& edge) {
                               return edge.first == to;
                           });
    return it != neighbors.end();
}

void DirectedWeightedGraph::addNode(int node) {
    if (nodeExists(node)) {
        throw std::invalid_argument(ERROR_NODE_ALREADY_EXISTS);
    }
    nodes_.insert(node);
    adjacency_list_[node] = std::vector<std::pair<int, double>>();
}

void DirectedWeightedGraph::removeNodeFromAllEdgesRecursive(
    std::map<int, std::vector<std::pair<int, double>>>::iterator it,
    int node) {
    if (it == adjacency_list_.end()) {
        return;
    }

    auto& neighbors = it->second;
    neighbors.erase(
        std::remove_if(neighbors.begin(), neighbors.end(),
                      [node](const std::pair<int, double>& edge) {
                          return edge.first == node;
                      }),
        neighbors.end()
    );

    removeNodeFromAllEdgesRecursive(std::next(it), node);
}

void DirectedWeightedGraph::removeNode(int node) {
    if (!nodeExists(node)) {
        throw std::invalid_argument(ERROR_NODE_NOT_FOUND);
    }

    removeNodeFromAllEdgesRecursive(adjacency_list_.begin(), node);

    auto it = adjacency_list_.find(node);
    if (it != adjacency_list_.end()) {
        it->second.clear();
        it->second.shrink_to_fit();
    }

    adjacency_list_.erase(node);
    nodes_.erase(node);
}

void DirectedWeightedGraph::addEdge(int from, int to, double weight) {
    if (!nodeExists(from) || !nodeExists(to)) {
        throw std::invalid_argument(ERROR_BOTH_NODES_NOT_FOUND);
    }

    if (from == to) {
        throw std::invalid_argument(ERROR_SELF_LOOP);
    }

    auto& neighbors = adjacency_list_[from];
    auto it = std::find_if(neighbors.begin(), neighbors.end(),
                          [to](const std::pair<int, double>& edge) {
                              return edge.first == to;
                          });

    if (it != neighbors.end()) {
        it->second = weight;
    } else {
        neighbors.push_back({to, weight});
    }
}

double DirectedWeightedGraph::removeEdge(int from, int to) {
    if (!nodeExists(from) || !nodeExists(to)) {
        throw std::invalid_argument(ERROR_BOTH_NODES_NOT_FOUND);
    }

    auto& neighbors = adjacency_list_[from];
    auto it = std::find_if(neighbors.begin(), neighbors.end(),
                          [to](const std::pair<int, double>& edge) {
                              return edge.first == to;
                          });

    if (it == neighbors.end()) {
        throw std::invalid_argument(ERROR_EDGE_NOT_FOUND);
    }

    double weight = it->second;
    neighbors.erase(it);

    if (neighbors.empty()) {
        neighbors.shrink_to_fit();
    }

    return weight;
}

double DirectedWeightedGraph::getEdgeWeight(int from, int to) const {
    if (!nodeExists(from) || !nodeExists(to)) {
        throw std::invalid_argument(ERROR_BOTH_NODES_NOT_FOUND);
    }

    const auto& neighbors = adjacency_list_.at(from);
    auto it = std::find_if(neighbors.begin(), neighbors.end(),
                          [to](const std::pair<int, double>& edge) {
                              return edge.first == to;
                          });

    if (it == neighbors.end()) {
        throw std::invalid_argument(ERROR_EDGE_NOT_FOUND);
    }

    return it->second;
}

std::set<int> DirectedWeightedGraph::getNodes() const {
    return nodes_;
}

std::vector<std::pair<int, double>> DirectedWeightedGraph::getNeighbors(int node) const {
    if (!nodeExists(node)) {
        throw std::invalid_argument(ERROR_NODE_NOT_FOUND);
    }
    return adjacency_list_.at(node);
}

size_t DirectedWeightedGraph::getNodeCount() const {
    return nodes_.size();
}

size_t DirectedWeightedGraph::getEdgeCount() const {
    size_t count = 0;
    for (const auto& pair : adjacency_list_) {
        count += pair.second.size();
    }
    return count;
}

void DirectedWeightedGraph::printNeighborsRecursive(
    const std::vector<std::pair<int, double>>& neighbors,
    size_t index) const {
    if (index >= neighbors.size()) {
        return;
    }

    std::cout << neighbors[index].first << "[" << neighbors[index].second << "]";
    if (index < neighbors.size() - 1) {
        std::cout << ", ";
    }

    printNeighborsRecursive(neighbors, index + 1);
}

void DirectedWeightedGraph::printGraphRecursive(
    std::set<int>::const_iterator it,
    const std::set<int>& nodes) const {
    if (it == nodes.end()) {
        return;
    }

    int node = *it;
    std::cout << "  " << node << " -> ";
    auto neighbors = getNeighbors(node);
    if (neighbors.empty()) {
        std::cout << "(no outgoing edges)";
    } else {
        printNeighborsRecursive(neighbors, 0);
    }
    std::cout << std::endl;

    printGraphRecursive(std::next(it), nodes);
}

void DirectedWeightedGraph::print() const {
    if (isEmpty()) {
        std::cout << "Graph is empty.\n";
        return;
    }
    std::cout << "\nCurrent graph:\n";
    printGraphRecursive(nodes_.begin(), nodes_);
    std::cout << std::endl;
}
