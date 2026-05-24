#ifndef WAVE_ALGORITHM_H
#define WAVE_ALGORITHM_H

#include "graph.h"
#include <map>
#include <vector>
#include <utility>

std::pair<std::map<int, int>, bool> waveShortestPath(
    const DirectedWeightedGraph& graph, int source, int target);

std::vector<int> reconstructPath(
    const DirectedWeightedGraph& graph,
    const std::map<int, int>& predecessors,
    int target);

#endif
