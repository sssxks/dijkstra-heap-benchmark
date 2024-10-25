#pragma once

#include <vector>
#include <limits>

const int INF = std::numeric_limits<int>::max();

struct Edge {
    int to, weight;
};

void dijkstra(int start, const std::vector<std::vector<Edge>>& graph, std::vector<int>& distances);
