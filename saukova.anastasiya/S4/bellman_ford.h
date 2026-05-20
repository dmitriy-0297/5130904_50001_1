#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "graph.h"
#include <map>
#include <utility>

std::pair<std::map<int, double>, std::map<int, int>> bellmanFordShortestPath(
    const DirectedWeightedGraph& graph, int source);

#endif
