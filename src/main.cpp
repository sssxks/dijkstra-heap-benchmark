#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <limits>
#include <chrono>
#include <functional>

#include "pathfinder.hpp"
#include "fiboheap.hpp"
#include "referenceheap.hpp"
#include "binaryheap.hpp"
#include "skewheap.hpp"

using namespace std;
using namespace std::chrono;

vector<vector<Edge>> readGraphFromFile(const string &filename)
{
    vector<vector<Edge>> graph;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    string line;
    int n = 0, m = 0; // n: number of nodes, m: number of arcs

    // Pre-allocate a buffer for reading lines
    line.reserve(256); // Assuming lines won't exceed 256 characters

    while (getline(file, line))
    {
        if (line.empty() || line[0] == 'c') // Skip comments
        {
            continue;
        }

        if (line[0] == 'p') // Parse size info
        {
            // Use sscanf for faster parsing
            sscanf(line.c_str(), "p %*s %d %d", &n, &m);
            graph.resize(n); // Pre-allocate space for the graph
            continue;
        }

        if (line[0] == 'a') // Parse arc information
        {
            int u, v, w;
            // Use sscanf for faster parsing
            sscanf(line.c_str(), "a %d %d %d", &u, &v, &w);
            graph[u - 1].push_back({v - 1, w});
        }
    }

    file.close();
    return graph;
}


vector<pair<int, int>> readQueriesFromFile(const string &filename)
{
    vector<pair<int, int>> queries;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    int start, end;
    while (file >> start >> end)
    {
        queries.emplace_back(start - 1, end - 1);
    }

    file.close();
    return queries;
}

template <class T>
void runDijkstra(vector<vector<Edge>> &graph, const vector<pair<int, int>> &queries)
{
    int64_t total_time = 0;
    for (const auto &query : queries)
    {
        int start = query.first;
        int end = query.second;
        auto start_time = high_resolution_clock::now();
        int shortest_distance = dijkstra<T>(start, end, graph);
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time).count();
        total_time += duration;

        cout << "Shortest distance from vertex " << start + 1 << " to vertex " << end + 1 << ": ";
        if (shortest_distance == INF)
        {
            cout << "INF" << endl;
        }
        else
        {
            cout << shortest_distance << endl;
        }
    }
    cout << "Total time: " << total_time << " microseconds" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <graph_filename> <queries_filename>" << endl;
        return 1;
    }

    string graph_filename = argv[1];
    string queries_filename = argv[2];

    auto graph = readGraphFromFile(graph_filename);
    auto queries = readQueriesFromFile(queries_filename);

    cout << "Fibonacci Heap:" << endl;
    runDijkstra<FiboHeap<NearestRecord>>(graph, queries);
    cout << "Reference Heap:" << endl;
    runDijkstra<ReferenceHeap<NearestRecord>>(graph, queries);
    cout << "Binary Heap:" << endl;
    runDijkstra<BinaryHeap<NearestRecord>>(graph, queries);
    cout << "Skew Heap:" << endl;
    runDijkstra<SkewHeap<NearestRecord>>(graph, queries);
    return 0;
}