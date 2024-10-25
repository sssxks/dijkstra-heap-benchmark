#pragma once

#include <vector>
#include <limits>

const int INF = std::numeric_limits<int>::max();

struct Edge
{
    int to, weight;
};

struct NearestRecord
{
    int dist, vertex;

    bool operator<(const NearestRecord &other) const
    {
        return dist < other.dist;
    }

    bool operator>(const NearestRecord &other) const
    {
        return dist > other.dist;
    }
};

template <typename PriorityQueue>
void dijkstra(int start, const std::vector<std::vector<Edge>> &graph, std::vector<int> &distances)
{
    auto n = graph.size();
    distances.assign(n, INF);
    distances[start] = 0;

    PriorityQueue pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        auto [dist, u] = pq.pop();

        if (dist > distances[u])
            continue;

        for (const Edge &edge : graph[u])
        {
            int v = edge.to;
            int weight = edge.weight;

            if (distances[u] + weight < distances[v])
            {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
}
