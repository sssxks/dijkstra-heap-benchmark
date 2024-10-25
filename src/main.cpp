#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>

#include "pathfinder.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Failed to open the file: " << argv[1] << endl;
        return 1;
    }

    int n, m;
    file >> n >> m;

    vector<vector<Edge>> graph(n);
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p') {
            continue;
        }

        assert(line[0] == 'a');
        istringstream iss(line.substr(2));
        int u, v, w;
        iss >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    file.close();

    int start;
    cout << "Enter start vertex: ";
    cin >> start;

    vector<int> distances;
    dijkstra(start, graph, distances);

    cout << "Shortest distances from vertex " << start << ":" << endl;
    for (int i = 0; i < n; ++i) {
        if (distances[i] == INF) {
            cout << "Vertex " << i << ": INF" << endl;
        } else {
            cout << "Vertex " << i << ": " << distances[i] << endl;
        }
    }

    return 0;
}