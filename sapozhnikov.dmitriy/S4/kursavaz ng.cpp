#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <functional>
#include <utility>
#include <iomanip>

static const char ERR_REMOVE_VERTEX_DOES_NOT_EXIST[] = "removeVertex: Vertex does not exist";
static const char ERR_SET_EDGE_VERTEX_DOES_NOT_EXIST[] = "setEdge: from/to Vertex does not exist";
static const char ERR_EDGES_FROM_VERTEX_DOES_NOT_EXIST[] = "edgesFrom: vertex does not exist";

static const char ERR_BFS_START_TARGET_DOES_NOT_EXIST[] = "BFS: start/target does not exist";
static const char ERR_DIJKSTRA_START_DOES_NOT_EXIST[] = "dijkstra: start vertex does not exist";
static const char ERR_DIJKSTRA_NEGATIVE_EDGE[] = "dijkstra: negative edge detected";
static const char ERR_BELLMAN_FORD_START_DOES_NOT_EXIST[] = "bellmanFord: start vertex does not exist";

class DirectedWeightedGraph {
public:
    typedef int Vertex;

    struct Edge {
        int weight;
        Vertex to;
    };

    DirectedWeightedGraph() {}

    bool hasVertices() const {
        return !graph_.empty();
    }

    bool hasVertex(Vertex v) const {
        return graph_.find(v) != graph_.end();
    }

    void addVertex(Vertex v) {
        if (!hasVertex(v)) {
            graph_[v] = std::vector<Edge>();
        }
    }

    void removeVertex(Vertex v) {
        if (!hasVertex(v)) {
            throw std::invalid_argument(ERR_REMOVE_VERTEX_DOES_NOT_EXIST);
        }

        graph_.erase(v);

        for (typename std::unordered_map<Vertex, std::vector<Edge>>::iterator it = graph_.begin(); it != graph_.end(); ++it) {
            std::vector<Edge>& edges = it->second;
            edges.erase(
                std::remove_if(edges.begin(), edges.end(),
                    [v](const Edge& e) { return e.to == v; }),
                edges.end()
            );
        }
    }

    bool hasEdge(Vertex from, Vertex to) const {
        typename std::unordered_map<Vertex, std::vector<Edge>>::const_iterator it = graph_.find(from);
        if (it == graph_.end()) return false;

        const std::vector<Edge>& edges = it->second;
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i].to == to) return true;
        }
        return false;
    }

    void setEdge(Vertex from, Vertex to, int weight) {
        if (!hasVertex(from) || !hasVertex(to)) {
            throw std::invalid_argument(ERR_SET_EDGE_VERTEX_DOES_NOT_EXIST);
        }

        std::vector<Edge>& edges = graph_[from];
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i].to == to) {
                edges[i].weight = weight;
                return;
            }
        }
        edges.push_back({ weight, to });
    }

    bool removeEdge(Vertex from, Vertex to) {
        typename std::unordered_map<Vertex, std::vector<Edge>>::iterator it = graph_.find(from);
        if (it == graph_.end()) return false;

        std::vector<Edge>& edges = it->second;
        for (typename std::vector<Edge>::iterator it2 = edges.begin(); it2 != edges.end(); ++it2) {
            if (it2->to == to) {
                edges.erase(it2);
                return true;
            }
        }
        return false;
    }

    const std::vector<Edge>& edgesFrom(Vertex v) const {
        typename std::unordered_map<Vertex, std::vector<Edge>>::const_iterator it = graph_.find(v);
        if (it == graph_.end()) {
            throw std::invalid_argument(ERR_EDGES_FROM_VERTEX_DOES_NOT_EXIST);
        }
        return it->second;
    }

    std::vector<Vertex> vertices() const {
        std::vector<Vertex> vs;
        vs.reserve(graph_.size());
        for (typename std::unordered_map<Vertex, std::vector<Edge>>::const_iterator it = graph_.begin(); it != graph_.end(); ++it) {
            vs.push_back(it->first);
        }
        return vs;
    }

    int edgeCount() const {
        int count = 0;
        for (typename std::unordered_map<Vertex, std::vector<Edge>>::const_iterator it = graph_.begin(); it != graph_.end(); ++it) {
            count += it->second.size();
        }
        return count;
    }

private:
    std::unordered_map<Vertex, std::vector<Edge>> graph_;
};

struct DijkstraResult {
    std::unordered_map<int, long long> dist;
    std::unordered_map<int, int> parent;
};

struct BellmanFordResult {
    std::unordered_map<int, long long> dist;
    std::unordered_map<int, int> parent;
    bool hasNegativeCycle;
};

std::vector<int> reconstructPath(int start, int target, const std::unordered_map<int, int>& parent) {
    std::vector<int> path;

    if (start == target) {
        path.push_back(start);
        return path;
    }

    if (parent.find(target) == parent.end()) {
        return std::vector<int>();
    }

    std::unordered_set<int> seen;
    int current = target;

    while (current != start) {
        if (seen.find(current) != seen.end()) {
            return std::vector<int>();
        }
        seen.insert(current);
        path.push_back(current);

        std::unordered_map<int, int>::const_iterator it = parent.find(current);
        if (it == parent.end()) return std::vector<int>();
        current = it->second;
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> BFS(const DirectedWeightedGraph& g, int start, int target) {
    if (!g.hasVertex(start) || !g.hasVertex(target)) {
        throw std::invalid_argument(ERR_BFS_START_TARGET_DOES_NOT_EXIST);
    }

    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == target) break;

        const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(u);
        for (size_t i = 0; i < edges.size(); ++i) {
            int v = edges[i].to;
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (visited.find(target) == visited.end()) return std::vector<int>();
    return reconstructPath(start, target, parent);
}

DijkstraResult dijkstra(const DirectedWeightedGraph& g, int start) {
    if (!g.hasVertex(start)) {
        throw std::invalid_argument(ERR_DIJKSTRA_START_DOES_NOT_EXIST);
    }

    const long long INF = std::numeric_limits<long long>::max() / 4;
    DijkstraResult res;

    std::vector<int> vs = g.vertices();
    for (size_t i = 0; i < vs.size(); ++i) {
        res.dist[vs[i]] = INF;
    }
    res.dist[start] = 0;

    typedef std::pair<long long, int> Item;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item> > pq;
    pq.push(Item(0, start));

    while (!pq.empty()) {
        Item top = pq.top();
        pq.pop();

        long long curDist = top.first;
        int u = top.second;

        if (curDist != res.dist[u]) continue;

        const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(u);
        for (size_t i = 0; i < edges.size(); ++i) {
            int v = edges[i].to;
            int w = edges[i].weight;

            if (w < 0) {
                throw std::logic_error(ERR_DIJKSTRA_NEGATIVE_EDGE);
            }

            long long nd = curDist + (long long)w;
            if (nd < res.dist[v]) {
                res.dist[v] = nd;
                res.parent[v] = u;
                pq.push(Item(nd, v));
            }
        }
    }

    return res;
}

BellmanFordResult bellmanFord(const DirectedWeightedGraph& g, int start) {
    if (!g.hasVertex(start)) {
        throw std::invalid_argument(ERR_BELLMAN_FORD_START_DOES_NOT_EXIST);
    }

    const long long INF = std::numeric_limits<long long>::max() / 4;
    BellmanFordResult res;
    res.hasNegativeCycle = false;

    std::vector<int> vs = g.vertices();
    for (size_t i = 0; i < vs.size(); ++i) {
        res.dist[vs[i]] = INF;
    }
    res.dist[start] = 0;

    struct EdgeInfo { int u, v, w; };
    std::vector<EdgeInfo> edges;

    for (size_t i = 0; i < vs.size(); ++i) {
        int u = vs[i];
        const std::vector<DirectedWeightedGraph::Edge>& out = g.edgesFrom(u);
        for (size_t j = 0; j < out.size(); ++j) {
            EdgeInfo ei;
            ei.u = u;
            ei.v = out[j].to;
            ei.w = out[j].weight;
            edges.push_back(ei);
        }
    }

    int V = vs.size();
    for (int i = 1; i < V; ++i) {
        bool changed = false;
        for (size_t j = 0; j < edges.size(); ++j) {
            if (res.dist[edges[j].u] >= INF) continue;
            long long nd = res.dist[edges[j].u] + (long long)edges[j].w;
            if (nd < res.dist[edges[j].v]) {
                res.dist[edges[j].v] = nd;
                res.parent[edges[j].v] = edges[j].u;
                changed = true;
            }
        }
        if (!changed) break;
    }

    for (size_t j = 0; j < edges.size(); ++j) {
        if (res.dist[edges[j].u] >= INF) continue;
        if (res.dist[edges[j].u] + (long long)edges[j].w < res.dist[edges[j].v]) {
            res.hasNegativeCycle = true;
            break;
        }
    }

    return res;
}

void printPath(const std::vector<int>& path, std::ostream& out = std::cout) {
    if (path.empty()) {
        out << "No path exists" << std::endl;
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) out << " -> ";
        out << path[i];
    }
    out << std::endl;
}

void printHelp() {
    std::cout << "add_vertex <v>                 - Add vertex v\n";
    std::cout << "remove_vertex <v>              - Remove vertex v\n";
    std::cout << "add_edge <from> <to> <weight>  - Add or update edge\n";
    std::cout << "remove_edge <from> <to>        - Remove edge\n";
    std::cout << "bfs <start> <target>           - BFS shortest path (by edges count)\n";
    std::cout << "dijkstra <start> <target>      - Dijkstra's algorithm (non-negative weights)\n";
    std::cout << "bellman_ford <start> <target>  - Bellman-Ford algorithm (handles negative weights)\n";
    std::cout << "vertices                       - List all vertices\n";
    std::cout << "edges [v]                      - List all edges or edges from vertex v\n";
    std::cout << "load <filename>                - Load graph from file\n";
    std::cout << "save <filename>                - Save graph to file\n";
    std::cout << "clear                          - Clear graph\n";
    std::cout << "info                           - Show graph info\n";
    std::cout << "help                           - Show this help\n";
    std::cout << "exit / quit                    - Exit program\n";
}

void saveGraph(const DirectedWeightedGraph& g, const std::string& filename) {
    std::ofstream fout(filename.c_str());
    if (!fout) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return;
    }

    fout << g.edgeCount() << "\n";

    std::vector<int> vs = g.vertices();
    for (size_t i = 0; i < vs.size(); ++i) {
        int u = vs[i];
        const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(u);
        for (size_t j = 0; j < edges.size(); ++j) {
            fout << u << " " << edges[j].to << " " << edges[j].weight << "\n";
        }
    }

    std::cout << "Graph saved to " << filename << std::endl;
}

void loadGraph(DirectedWeightedGraph& g, const std::string& filename) {
    std::ifstream fin(filename.c_str());
    if (!fin) {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return;
    }

    DirectedWeightedGraph newGraph;
    int m;
    fin >> m;

    if (!fin || m < 0) {
        std::cerr << "Error: Invalid file format" << std::endl;
        return;
    }

    for (int i = 0; i < m; ++i) {
        int from, to, w;
        fin >> from >> to >> w;
        if (!fin) {
            std::cerr << "Error: Invalid edge data at line " << (i + 2) << std::endl;
            return;
        }
        newGraph.addVertex(from);
        newGraph.addVertex(to);
        newGraph.setEdge(from, to, w);
    }

    g = newGraph;
    std::cout << "Graph loaded from " << filename << std::endl;
    std::cout << "Vertices: " << g.vertices().size() << ", Edges: " << g.edgeCount() << std::endl;
}

void printVertices(const DirectedWeightedGraph& g) {
    std::vector<int> vs = g.vertices();
    if (vs.empty()) {
        std::cout << "No vertices in graph" << std::endl;
        return;
    }

    std::sort(vs.begin(), vs.end());
    std::cout << "Vertices (" << vs.size() << "): ";
    for (size_t i = 0; i < vs.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << vs[i];
    }
    std::cout << std::endl;
}

void printEdges(const DirectedWeightedGraph& g, int vertex = -1) {
    if (vertex != -1) {
        if (!g.hasVertex(vertex)) {
            std::cout << "Vertex " << vertex << " does not exist" << std::endl;
            return;
        }

        const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(vertex);
        std::cout << "Edges from " << vertex << ": ";
        if (edges.empty()) {
            std::cout << "none";
        }
        else {
            for (size_t i = 0; i < edges.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << vertex << " -> " << edges[i].to << " (w=" << edges[i].weight << ")";
            }
        }
        std::cout << std::endl;
    }
    else {
        bool hasEdges = false;
        std::vector<int> vs = g.vertices();
        std::sort(vs.begin(), vs.end());

        for (size_t i = 0; i < vs.size(); ++i) {
            int u = vs[i];
            const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(u);
            for (size_t j = 0; j < edges.size(); ++j) {
                std::cout << "  " << u << " -> " << edges[j].to << " (w=" << edges[j].weight << ")" << std::endl;
                hasEdges = true;
            }
        }

        if (!hasEdges) {
            std::cout << "No edges in graph" << std::endl;
        }
    }
}

void printGraphInfo(const DirectedWeightedGraph& g) {
    int V = g.vertices().size();
    int E = g.edgeCount();

    std::cout << "Vertices count: " << V << std::endl;
    std::cout << "Edges count: " << E << std::endl;

    if (V > 0 && V <= 20) {
        printVertices(g);
    }

    bool hasNegative = false;
    std::vector<int> vs = g.vertices();
    for (size_t i = 0; i < vs.size() && !hasNegative; ++i) {
        const std::vector<DirectedWeightedGraph::Edge>& edges = g.edgesFrom(vs[i]);
        for (size_t j = 0; j < edges.size() && !hasNegative; ++j) {
            if (edges[j].weight < 0) {
                hasNegative = true;
            }
        }
    }
    std::cout << "Has negative weights: " << (hasNegative ? "yes" : "no") << std::endl;
}

int main() {
    DirectedWeightedGraph g;
    std::string line;

    std::cout << "Directed Weighted Graph Interactive Console" << std::endl;
    std::cout << "Type 'help' for list of commands, 'exit' to quit." << std::endl;

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, line);

        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "exit" || cmd == "quit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "add_vertex") {
            int v;
            if (iss >> v) {
                g.addVertex(v);
                std::cout << "Vertex " << v << " added" << std::endl;
            }
            else {
                std::cout << "Usage: add_vertex <v>" << std::endl;
            }
        }
        else if (cmd == "remove_vertex") {
            int v;
            if (iss >> v) {
                try {
                    g.removeVertex(v);
                    std::cout << "Vertex " << v << " removed" << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: remove_vertex <v>" << std::endl;
            }
        }
        else if (cmd == "add_edge") {
            int from, to, w;
            if (iss >> from >> to >> w) {
                try {
                    g.addVertex(from);
                    g.addVertex(to);
                    g.setEdge(from, to, w);
                    std::cout << "Edge " << from << " -> " << to << " (w=" << w << ") added/updated" << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: add_edge <from> <to> <weight>" << std::endl;
            }
        }
        else if (cmd == "remove_edge") {
            int from, to;
            if (iss >> from >> to) {
                if (g.removeEdge(from, to)) {
                    std::cout << "Edge " << from << " -> " << to << " removed" << std::endl;
                }
                else {
                    std::cout << "Edge " << from << " -> " << to << " not found" << std::endl;
                }
            }
            else {
                std::cout << "Usage: remove_edge <from> <to>" << std::endl;
            }
        }
        else if (cmd == "bfs") {
            int start, target;
            if (iss >> start >> target) {
                try {
                    std::vector<int> path = BFS(g, start, target);
                    std::cout << "BFS path (" << start << " -> " << target << "): ";
                    printPath(path);
                    if (!path.empty()) {
                        std::cout << "Number of edges: " << path.size() - 1 << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: bfs <start> <target>" << std::endl;
            }
        }
        else if (cmd == "dijkstra") {
            int start, target;
            if (iss >> start >> target) {
                try {
                    if (!g.hasVertex(start) || !g.hasVertex(target)) {
                        std::cout << "Error: Start or target vertex does not exist" << std::endl;
                        continue;
                    }

                    DijkstraResult res = dijkstra(g, start);
                    if (res.dist.find(target) != res.dist.end() && res.dist[target] < std::numeric_limits<long long>::max() / 4) {
                        std::cout << "Distance: " << res.dist[target] << std::endl;
                        std::cout << "Path: ";
                        printPath(reconstructPath(start, target, res.parent));
                    }
                    else {
                        std::cout << "No path from " << start << " to " << target << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: dijkstra <start> <target>" << std::endl;
            }
        }
        else if (cmd == "bellman_ford") {
            int start, target;
            if (iss >> start >> target) {
                try {
                    if (!g.hasVertex(start) || !g.hasVertex(target)) {
                        std::cout << "Error: Start or target vertex does not exist" << std::endl;
                        continue;
                    }

                    BellmanFordResult res = bellmanFord(g, start);
                    if (res.hasNegativeCycle) {
                        std::cout << "Graph contains a negative cycle! Shortest paths are not well-defined." << std::endl;
                    }
                    else if (res.dist.find(target) != res.dist.end() && res.dist[target] < std::numeric_limits<long long>::max() / 4) {
                        std::cout << "Distance: " << res.dist[target] << std::endl;
                        std::cout << "Path: ";
                        printPath(reconstructPath(start, target, res.parent));
                    }
                    else {
                        std::cout << "No path from " << start << " to " << target << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: bellman_ford <start> <target>" << std::endl;
            }
        }
        else if (cmd == "vertices") {
            printVertices(g);
        }
        else if (cmd == "edges") {
            int v;
            if (iss >> v) {
                printEdges(g, v);
            }
            else {
                printEdges(g);
            }
        }
        else if (cmd == "clear") {
            g = DirectedWeightedGraph();
            std::cout << "Graph cleared" << std::endl;
        }
        else if (cmd == "info") {
            printGraphInfo(g);
        }
        else if (cmd == "load") {
            std::string filename;
            if (iss >> filename) {
                loadGraph(g, filename);
            }
            else {
                std::cout << "Usage: load <filename>" << std::endl;
            }
        }
        else if (cmd == "save") {
            std::string filename;
            if (iss >> filename) {
                saveGraph(g, filename);
            }
            else {
                std::cout << "Usage: save <filename>" << std::endl;
            }
        }
        else {
            std::cout << "Unknown command: " << cmd << ". Type 'help' for available commands." << std::endl;
        }
    }

    return 0;
}