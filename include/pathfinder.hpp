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
 * @brief Contains the implementation of Dijkstra's algorithm for finding the shortest paths in a graph.
 * 
 * @tparam PriorityQueue The type of priority queue to be used in the algorithm. It must support push and pop operations.
 * 
 * @param start The starting vertex for Dijkstra's algorithm.
 * @param graph A reference to the adjacency list representation of the graph. Each element in the outer vector represents a vertex, and each inner vector contains edges (of type Edge) originating from that vertex.
 * @param distances A reference to a vector where the shortest distances from the start vertex to each vertex will be stored. The vector will be resized to match the number of vertices in the graph.
 * 
 * The function uses a priority queue to efficiently find the shortest paths from the start vertex to all other vertices in the graph. The distances vector will be updated with the shortest distances, where distances[i] represents the shortest distance from the start vertex to vertex i.
 */
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
        // get the vertex with the smallest distance
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
