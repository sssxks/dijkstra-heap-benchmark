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

vector<vector<Edge>> graph;
int n;

void readGraphFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty() || line[0] == 'c' || line[0] == 'p')
        {
            continue;
        }

        assert(line[0] == 'a');
        istringstream iss(line.substr(2));
        int u, v, w;
        iss >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    file.close();
}

void runDijkstra()
{
    int start;
    cout << "Enter start vertex: ";
    cin >> start;

    vector<int> distances;
    dijkstra<ReferenceHeap<NearestRecord>>(start, graph, distances);

    cout << "Shortest distances from vertex " << start << ":" << endl;
    for (int i = 0; i < n; ++i)
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
    readGraphFromFile(filename);
    runDijkstra();
    return 0;
}