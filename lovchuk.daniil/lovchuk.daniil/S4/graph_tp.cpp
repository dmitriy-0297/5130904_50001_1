#include "graph_tp.hpp"

Graph::Graph() : vertices(0) {}

void Graph::init(int n) {
    if (n < 0) throw invalid_argument("N < 0");
    vertices = n;
    adj.resize(n);
    dist.resize(n);
    parent.resize(n);
    visited.resize(n);
}

void Graph::addVertex() {
    adj.push_back(vector<int>());
    dist.push_back(-1);
    parent.push_back(-1);
    visited.push_back(false);
    vertices++;
}

void Graph::addEdge(int u, int v) {
    if (u < 0 || u >= vertices || v < 0 || v >= vertices)
        throw out_of_range("Invalid vertex index");
    if (u == v) throw invalid_argument("Self loops not allowed");

    if (find(adj[u].begin(), adj[u].end(), v) != adj[u].end())
        throw invalid_argument("Edge exists");

    adj[u].push_back(v);
    adj[v].push_back(u);
}

void Graph::removeEdge(int u, int v) {
    if (u < 0 || u >= vertices || v < 0 || v >= vertices) return;

    adj[u].erase(remove(adj[u].begin(), adj[u].end(), v), adj[u].end());
    adj[v].erase(remove(adj[v].begin(), adj[v].end(), u), adj[v].end());
}

void Graph::clear() {
    adj.clear();
    vertices = 0;
}

int Graph::countVertices() const { return vertices; }
bool Graph::isBlank() const { return vertices == 0; }

void Graph::processQueueRecursive(queue<int>& q) {
    if (q.empty()) return;

    int curr = q.front();
    q.pop();

    for_each(adj[curr].begin(), adj[curr].end(), [&](int neighbor) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            dist[neighbor] = dist[curr] + 1;
            parent[neighbor] = curr;
            order.push_back(neighbor);
            q.push(neighbor);
        }
        });

    processQueueRecursive(q);
}

void Graph::bfsRecursive(queue<int>& q, vector<int>& d) {
    if (q.empty()) return;
    int u = q.front();
    q.pop();

    for_each(adj[u].begin(), adj[u].end(), [&](int v) {
        if (d[v] == -1) {
            d[v] = d[u] + 1;
            q.push(v);
        }
        });
    bfsRecursive(q, d);
}

void Graph::executeBFS(int start) {
    if (start < 0 || start >= vertices) throw out_of_range("Invalid start");

    fill(dist.begin(), dist.end(), -1);
    fill(parent.begin(), parent.end(), -1);
    fill(visited.begin(), visited.end(), false);
    order.clear();

    queue<int> q;
    visited[start] = true;
    dist[start] = 0;
    order.push_back(start);
    q.push(start);

    processQueueRecursive(q);
}

void Graph::printBFSTree(int start) {
    executeBFS(start);
    cout << ">>> BFS Tree (root: " << start << ") <<<\n";

    for_each(order.begin(), order.end(), [&](int v) {
        cout << "  Vertex " << v;
        if (v == start) cout << " (root)\n";
        else cout << " <- parent " << parent[v] << " [dist=" << dist[v] << "]\n";
        });

    cout << "Order: ";
    std::copy(order.begin(), order.end(), ostream_iterator<int>(cout, " -> "));
    cout << "\n";
}

void Graph::printShortestPath(int u, int v) {
    executeBFS(u);
    if (dist[v] == -1) {
        cerr << "Error: No path from " << u << " to " << v << "\n";
        return;
    }

    vector<int> path;
    int curr = v;

    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }

    cout << "Route " << u << " => " << v << " (len: " << dist[v] << "): ";
    std::copy(path.begin(), path.end(), ostream_iterator<int>(cout, " -> "));
    cout << "\n";
}

int Graph::computeDiameter(int start) {
    if (vertices == 0) throw logic_error("Graph empty");

    vector<int> d1(vertices, -1);
    queue<int> q1;
    d1[start] = 0;
    q1.push(start);

    bfsRecursive(q1, d1);

    auto it1 = max_element(d1.begin(), d1.end());
    int end1 = distance(d1.begin(), it1);

    vector<int> d2(vertices, -1);
    queue<int> q2;
    d2[end1] = 0;
    q2.push(end1);
    bfsRecursive(q2, d2);

    auto it2 = max_element(d2.begin(), d2.end());
    if (it2 != d2.end()) return *it2;
    return 0;
}

void Graph::printDiameter(int start) {
    int diam = computeDiameter(start);
    cout << "Max depth (diameter) from " << start << ": " << diam << "\n";
}

void Graph::printGraph() {
    cout << "[GRAPH] Nodes: " << vertices << "\n";
    int idx = 0;
    for_each(adj.begin(), adj.end(), [&](const vector<int>& neighbors) {
        cout << "  [" << idx++ << "] -> ";
        std::copy(neighbors.begin(), neighbors.end(), ostream_iterator<int>(cout, ", "));
        cout << "\n";
        });
}
