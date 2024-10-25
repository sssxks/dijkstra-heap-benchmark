#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "pathfinder.hpp"

using namespace std;

struct QueryPair {
    int start, end;
};

int main() {
    ifstream inputFile("data.in");
    if (!inputFile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    int n, m, q;
    inputFile >> n >> m >> q;

    PGraph G = BuildAGraph(n, m);
    vector<QueryPair> queryPairs(q);

    for (int i = 0; i < m; ++i) {
        int f, t, d;
        inputFile >> f >> t >> d;
        AddEdge(G, f - 1, t - 1, d);
    }

    for (int i = 0; i < q; ++i) {
        int s, e;
        inputFile >> s >> e;
        queryPairs[i] = {s - 1, e - 1};
    }

    inputFile.close();

    auto runDijkstra = [&](const string& filename, auto dijkstraFunc) {
        ofstream outputFile(filename);
        if (!outputFile) {
            cerr << "Error opening output file: " << filename << endl;
            return;
        }

        outputFile << "The result using " << filename << " is: \n";
        auto begin = chrono::high_resolution_clock::now();

        for (const auto& query : queryPairs) {
            Dis ans = dijkstraFunc(G, query.start, query.end);
            outputFile << ans << '\n';
        }

        auto finish = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = finish - begin;
        outputFile << "The time using " << filename << " is: " << elapsed.count() << " seconds\n";
        outputFile.close();
    };

    runDijkstra("PriorityQueue.out", DijkstraUsingPriorityQueue);
    cout << "The function using Priority Queue completed!" << endl;

    runDijkstra("FibonacciHeap.out", DijkstraUsingFibonacciHeap);
    cout << "The function using Fibonacci Heap completed!" << endl;

    return 0;
}