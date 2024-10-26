#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <limits>

#include "pathfinder.hpp"
#include "fiboheap.hpp"
#include "referenceheap.hpp"

using namespace std;

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
    while (getline(file, line))
    {
        if (line.empty() || line[0] == 'c') // c for comment
        {
            continue;
        }

        if (line[0] == 'p') // p for size info
        {
            istringstream iss(line.substr(4));
            int n, m;
            iss >> n >> m; // n : nodes, m : arcs

            graph.resize(n, vector<Edge>());
            continue;
        }

        assert(line[0] == 'a'); // a for arc
        istringstream iss(line.substr(2));
        int u, v, w;
        iss >> u >> v >> w;
        graph[u - 1].push_back({v - 1, w});
    }

    file.close();
    return graph;
}

vector<int> readQueriesFromFile(const string &filename)
{
    vector<int> queries;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    int query;
    while (file >> query)
    {
        queries.push_back(query - 1);
    }

    file.close();
    return queries;
}

void runDijkstra(vector<vector<Edge>> &graph, const vector<int> &queries)
{
    for (int start : queries)
    {
        vector<int> distances;
        dijkstra<FiboHeap<NearestRecord>>(start, graph, distances);

        cout << "Shortest distances from vertex " << start + 1 << ":" << endl;
        for (long unsigned int i = 0; i < graph.size(); ++i)
        {
            if (distances[i] == INF)
            {
                cout << "Vertex " << i + 1 << ": INF" << endl;
            }
            else
            {
                cout << "Vertex " << i + 1 << ": " << distances[i] << endl;
            }
        }
    }
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

    runDijkstra(graph, queries);
    return 0;
}