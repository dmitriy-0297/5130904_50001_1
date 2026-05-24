#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <map>
#include <utility>

std::pair<std::map<int, double>, std::map<int, int>> dijkstraShortestPath(
    const DirectedWeightedGraph& graph, int source);

#endif
