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
        graph[u-1].push_back({v-1, w});
    }

    file.close();
    return graph;
}

void runDijkstra(vector<vector<Edge>> &graph)
{
    int start;
    cout << "Enter start vertex: ";
    cin >> start;

    vector<int> distances;
    dijkstra<FiboHeap<NearestRecord>>(start, graph, distances);

    cout << "Shortest distances from vertex " << start << ":" << endl;
    for (long unsigned int i = 0; i < graph.size(); ++i)
    {
        if (distances[i] == INF)
        {
            cout << "Vertex " << i << ": INF" << endl;
        }
        else
        {
            cout << "Vertex " << i << ": " << distances[i] << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    auto graph = readGraphFromFile(filename);
    runDijkstra(graph);
    return 0;
}