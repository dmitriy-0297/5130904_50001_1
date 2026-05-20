#include "Graph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <iterator>
#include <stdexcept>

// Константы ошибок
static const char* const ERR_INVALID_VERTEX =
    "Ошибка: Неверный индекс вершины!";
static const char* const ERR_EMPTY_GRAPH =
    "Ошибка: Граф пуст!";
static const char* const ERR_SELF_LOOP =
    "Ошибка: Петли запрещены!";
static const char* const ERR_NEGATIVE_VERTICES =
    "Ошибка: Количество вершин должно быть целым неотрицательным числом";
static const char* const ERR_EDGE_EXISTS =
    "Ошибка: Ребро уже существует!";

// Вспомогательные методы
void Graph::resetArrays() {
    std::fill(dist_.begin(), dist_.end(), -1);
    std::fill(parent_.begin(), parent_.end(), -1);
    std::fill(visited_.begin(), visited_.end(), false);
    order_.clear();
}

void Graph::findDiameterPath(int start, std::vector<int>& outPath,
                             int& outDiameter) {
    bfs(start);
    std::vector<int> treeParent = parent_;

    int end1 = static_cast<int>(std::max_element(dist_.begin(), dist_.end())
                                - dist_.begin());

    std::vector<int> dist2(vertices_, -1);
    std::vector<int> parent2(vertices_, -1);
    std::queue<int> q;
    dist2[end1] = 0;
    q.push(end1);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        std::for_each(adj_[curr].begin(), adj_[curr].end(),
            [&](int neighbor) {
                if ((treeParent[neighbor] == curr || treeParent[curr] == neighbor)
                    && dist2[neighbor] == -1) {
                    dist2[neighbor] = dist2[curr] + 1;
                    parent2[neighbor] = curr;
                    q.push(neighbor);
                }
            });
    }

    int end2 = static_cast<int>(std::max_element(dist2.begin(), dist2.end())
                                - dist2.begin());
    outDiameter = dist2[end2] < 0 ? 0 : dist2[end2];

    outPath.clear();
    for (int cur = end2; cur != -1; cur = parent2[cur])
        outPath.push_back(cur);
}

Graph::Graph() : vertices_(0) {}

Graph::Graph(int n) {
    if (n < 0) throw std::invalid_argument(ERR_NEGATIVE_VERTICES);
    vertices_ = n;
    adj_.assign(n, std::vector<int>());
    dist_.assign(n, -1);
    parent_.assign(n, -1);
    visited_.assign(n, false);
}

// Базовые операции
bool Graph::isEmpty() const {
    return vertices_ == 0;
}

bool Graph::hasVertex(int v) const {
    return v >= 0 && v < vertices_;
}

bool Graph::hasEdge(int u, int v) const {
    if (!hasVertex(u) || !hasVertex(v)) return false;
    return std::find(adj_[u].begin(), adj_[u].end(), v)
           != adj_[u].end();
}

void Graph::addVertex() {
    adj_.emplace_back();
    dist_.push_back(-1);
    parent_.push_back(-1);
    visited_.push_back(false);
    ++vertices_;
}

void Graph::removeVertex(int v) {
    if (!hasVertex(v))
        throw std::out_of_range(ERR_INVALID_VERTEX);

    // Удаляем все рёбра, ведущие к v
    std::for_each(adj_.begin(), adj_.end(),
        [v](std::vector<int>& nbrs) {
            nbrs.erase(std::remove(nbrs.begin(), nbrs.end(), v),
                       nbrs.end());
        });

    // Уменьшаем индексы вершин > v
    std::for_each(adj_.begin(), adj_.end(),
        [v](std::vector<int>& nbrs) {
            std::transform(nbrs.begin(), nbrs.end(), nbrs.begin(),
                [v](int x) { return x > v ? x - 1 : x; });
        });

    adj_.erase(adj_.begin() + v);
    dist_.erase(dist_.begin() + v);
    parent_.erase(parent_.begin() + v);
    visited_.erase(visited_.begin() + v);
    --vertices_;
    order_.clear();
}

void Graph::addEdge(int u, int v) {
    if (!hasVertex(u) || !hasVertex(v))
        throw std::out_of_range(ERR_INVALID_VERTEX);
    if (u == v)
        throw std::invalid_argument(ERR_SELF_LOOP);
    if (hasEdge(u, v))
        throw std::invalid_argument(ERR_EDGE_EXISTS);
    adj_[u].push_back(v);
    adj_[v].push_back(u);
}

void Graph::removeEdge(int u, int v) {
    if (!hasVertex(u) || !hasVertex(v)) return;
    adj_[u].erase(std::remove(adj_[u].begin(), adj_[u].end(), v),
                  adj_[u].end());
    adj_[v].erase(std::remove(adj_[v].begin(), adj_[v].end(), u),
                  adj_[v].end());
}

int Graph::getVertexCount() const {
    return vertices_;
}

void Graph::print() const {
    if (isEmpty()) {
        std::cout << "Граф пуст\n";
        return;
    }
    std::cout << "Граф (" << vertices_ << " вершин):\n";
    for (int i = 0; i < vertices_; ++i) {
        std::cout << "  " << i << ": ";
        std::copy(adj_[i].begin(), adj_[i].end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\n";
    }
}

// Обход в ширину (bfs)
void Graph::bfs(int start) {
    if (!hasVertex(start))
        throw std::out_of_range(ERR_INVALID_VERTEX);
    if (isEmpty())
        throw std::logic_error(ERR_EMPTY_GRAPH);

    resetArrays();
    std::queue<int> q;
    dist_[start] = 0;
    visited_[start] = true;
    q.push(start);
    order_.push_back(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        std::for_each(adj_[current].begin(), adj_[current].end(),
            [&](int neighbor) {
                if (!visited_[neighbor]) {
                    visited_[neighbor] = true;
                    dist_[neighbor] = dist_[current] + 1;
                    parent_[neighbor] = current;
                    q.push(neighbor);
                    order_.push_back(neighbor);
                }
            });
    }
}

void Graph::printBFSTree(int start) {
    std::cout << "\n\tДерево BFS от вершины " << start << "\n";
    if (isEmpty()) {
        std::cout << "Граф пуст\n\n";
        return;
    }

    bfs(start);

    for (int i = 0; i < vertices_; ++i) {
        if (i == start)
            std::cout << "  Вершина " << i << " (корень)\n";
        else if (parent_[i] != -1)
            std::cout << "  Вершина " << i << " <- родитель " << parent_[i]
                      << " (расстояние " << dist_[i] << ")\n";
        else
            std::cout << "  Вершина " << i << " (недостижима)\n";
    }

    std::cout << "\nПорядок обхода: ";
    std::copy(order_.begin(), order_.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

// 2. Кратчайший путь
void Graph::printShortestPath(int start, int target) {
    if (!hasVertex(start))
        throw std::out_of_range(ERR_INVALID_VERTEX);
    if (!hasVertex(target))
        throw std::out_of_range(ERR_INVALID_VERTEX);

    bfs(start);

    if (dist_[target] == -1) {
        std::cout << "Нет пути от " << start << " до " << target << "\n";
        return;
    }

    std::vector<int> path;
    for (int cur = target; cur != -1; cur = parent_[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());

    std::cout << "Путь от " << start << " до " << target << ": ";
    std::copy(path.begin(), path.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << "(длина: " << dist_[target] << ")\n";
}

// 3. Диаметр дерева BFS
int Graph::getDiameter(int start) {
    if (isEmpty())
        throw std::logic_error(ERR_EMPTY_GRAPH);
    if (!hasVertex(start))
        throw std::out_of_range(ERR_INVALID_VERTEX);
    std::vector<int> path;
    int diameter = 0;
    findDiameterPath(start, path, diameter);
    return diameter;
}

void Graph::printDiameterPath(int start) {
    if (isEmpty())
        throw std::logic_error(ERR_EMPTY_GRAPH);
    if (!hasVertex(start))
        throw std::out_of_range(ERR_INVALID_VERTEX);

    std::vector<int> path;
    int diameter = 0;
    findDiameterPath(start, path, diameter);

    std::cout << "\nДиаметр BFS-дерева: " << diameter << "\nПуть: ";
    std::copy(path.rbegin(), path.rend(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}
