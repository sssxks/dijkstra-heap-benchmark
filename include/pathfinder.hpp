#pragma once

#include <vector>
#include <limits>

// Constant representing infinity, used to initialize distances
const int INF = std::numeric_limits<int>::max();

// Structure representing an edge in the graph
struct Edge
{
    int to;     // The destination vertex of the edge
    int weight; // The weight of the edge
};

// Structure representing a record in the priority queue
struct NearestRecord
{
    int dist;   // The distance from the start vertex
    int vertex; // The vertex number

    // Comparison operator for less-than, used for priority queue ordering
    bool operator<(const NearestRecord &other) const
    {
        return dist < other.dist;
    }

    // Comparison operator for greater-than, used for priority queue ordering
    bool operator>(const NearestRecord &other) const
    {
        return dist > other.dist;
    }
};

/**
 * @file pathfinder.hpp
 * @brief Contains the implementation of Dijkstra's algorithm for finding the shortest path in a graph from a start vertex to a destination vertex.
 * 
 * @tparam PriorityQueue The type of priority queue to be used in the algorithm. It must support push and pop operations.
 * 
 * @param start The starting vertex for Dijkstra's algorithm.
 * @param destination The destination vertex for which the shortest path is to be found.
 * @param graph A reference to the adjacency list representation of the graph. Each element in the outer vector represents a vertex, and each inner vector contains edges (of type Edge) originating from that vertex.
 * 
 * @return The shortest path length from the start vertex to the destination vertex. If there is no path, returns INF.
 * 
 * The function uses a priority queue to efficiently find the shortest path from the start vertex to the destination vertex in the graph.
 */
template <typename PriorityQueue>
int dijkstra(int start, int destination, const std::vector<std::vector<Edge>> &graph)
{
    auto n = graph.size();
    std::vector<int> distances(n, INF);
    distances[start] = 0;

    PriorityQueue pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        // get the vertex with the smallest distance
        auto [dist, u] = pq.pop();

        if (u == destination)
            return dist;

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

    return INF; // If the destination is not reachable
}
