#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <limits>

struct Edge {
    int target;
    int weight;
};

class Graph {
public:
    void addEdge(int source, int target, int weight);
    std::unordered_map<int, std::vector<Edge>> adjList;
};

void Graph::addEdge(int source, int target, int weight) {
    Edge edge = {target, weight};
    adjList[source].push_back(edge);
}

Graph readGraphFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    Graph graph;
    std::string line;
    int edgeCount = 0;
    while (std::getline(file, line)) {
        if (line[0] == 'a') {
            std::istringstream iss(line.substr(2));
            int source, target, weight;
            if (!(iss >> source >> target >> weight)) {
                throw std::runtime_error("Failed to parse line");
            }
            graph.addEdge(source, target, weight);
            graph.addEdge(target, source, weight);
            edgeCount++;
        }
    }

    return graph;
}

template<typename HeapType>
int dijkstra(const Graph& graph, int startNode, int endNode, HeapType& heap) {
    std::unordered_map<int, int> distances;
    for (const auto& [node, _] : graph.adjList) {
        distances[node] = std::numeric_limits<int>::max();
    }
    distances[startNode] = 0;

    for (auto& [node, distance] : distances) {
        heap.insert(node, distance);
    }

    while (!heap.isEmpty()) {
        int currentNode = heap.extractMin();
        int currentDistance = distances[currentNode];

        for (const auto& edge : graph.adjList.at(currentNode)) {
            int neighbor = edge.target;
            int weight = edge.weight;
            int newDistance = currentDistance + weight;

            if (newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                heap.decreaseKey(neighbor, newDistance);
            }
        }

        if (currentNode == endNode) {
            break;
        }
    }

    return distances[endNode];
}

class FibonacciHeap {
public:
    struct Node {
        int key;
        int value;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        int degree;
        bool mark;
    };

    Node* minNode;
    int nodeCount;

    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    void insert(int node, int distance) {
        Node* newNode = new Node{distance, node, nullptr, nullptr, nullptr, nullptr, 0, false};
        if (minNode == nullptr) {
            minNode = newNode;
            newNode->left = newNode;
            newNode->right = newNode;
        } else {
            newNode->left = minNode;
            newNode->right = minNode->right;
            minNode->right->left = newNode;
            minNode->right = newNode;
            if (newNode->key < minNode->key) {
                minNode = newNode;
            }
        }
        nodeCount++;
        std::cout << "Inserted node " << node << " with distance " << distance << std::endl;
    }

    int extractMin() {
        if (minNode == nullptr) {
            throw std::runtime_error("Heap is empty");
        }
        int minKey = minNode->key;
        int minValue = minNode->value;
        Node* child = minNode->child;
        Node* nextChild = child;
        while (nextChild != nullptr) {
            nextChild->parent = nullptr;
            nextChild = nextChild->right;
            if (nextChild == child) {
                break;
            }
        }
        if (child != nullptr) {
            child->left->right = minNode->right;
            minNode->right->left = child->left;
        }
        minNode->left->right = minNode->right;
        minNode->right->left = minNode->left;
        minNode = minNode->right;
        if (minNode == minNode->left) {
            minNode = nullptr;
        } else {
            consolidate();
        }
        Node* temp = minNode;
        delete minNode;
        nodeCount--;
        std::cout << "Extracted min node " << minValue << " with distance " << minKey << std::endl;
        minNode = temp;
        return minValue;
    }

    void decreaseKey(int node, int newDistance) {
        Node* targetNode = findNode(node);
        if (targetNode == nullptr) {
            throw std::runtime_error("Node not found");
        }
        if (newDistance >= targetNode->key) {
            return;
        }
        targetNode->key = newDistance;
        Node* parent = targetNode->parent;
        if (parent != nullptr && targetNode->key < parent->key) {
            cut(targetNode, parent);
            cascadingCut(parent);
        }
        if (targetNode->key < minNode->key) {
            minNode = targetNode;
        }
        std::cout << "Decreased key of node " << node << " to " << newDistance << std::endl;
    }

    bool isEmpty() const {
        return minNode == nullptr;
    }

private:
    void consolidate() {
        int maxDegree = static_cast<int>(std::log(nodeCount) / std::log(2)) + 1;
        std::vector<Node*> A(maxDegree, nullptr);
        Node* w = minNode;
        do {
            Node* x = w;
            int d = x->degree;
            while (A[d] != nullptr) {
                Node* y = A[d];
                if (x->key > y->key) {
                    std::swap(x, y);
                }
                link(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            w = w->right;
        } while (w != minNode);

        minNode = nullptr;
        for (int i = 0; i < maxDegree; i++) {
            if (A[i] != nullptr) {
                if (minNode == nullptr) {
                    minNode = A[i];
                    A[i]->left = A[i];
                    A[i]->right = A[i];
                } else {
                    A[i]->right = minNode->right;
                    A[i]->left = minNode;
                    minNode->right->left = A[i];
                    minNode->right = A[i];
                    if (A[i]->key < minNode->key) {
                        minNode = A[i];
                    }
                }
            }
        }
        std::cout << "Consolidated heap" << std::endl;
    }

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;
        if (x->child == nullptr) {
            x->child = y;
            y->right = y;
            y->left = y;
        } else {
            y->right = x->child;
            y->left = x->child->left;
            x->child->left->right = y;
            x->child->left = y;
        }
        x->degree++;
        y->mark = false;
        std::cout << "Linked node " << y->value << " to node " << x->value << std::endl;
    }

    void cut(Node* x, Node* y) {
        x->left->right = x->right;
        x->right->left = x->left;
        y->degree--;
        if (y->child == x) {
            y->child = x->right;
        }
        if (y->degree == 0) {
            y->child = nullptr;
        }
        x->right = minNode->right;
        x->left = minNode;
        minNode->right->left = x;
        minNode->right = x;
        x->parent = nullptr;
        x->mark = false;
        std::cout << "Cut node " << x->value << " from node " << y->value << std::endl;
    }

    void cascadingCut(Node* y) {
        Node* z = y->parent;
        if (z != nullptr) {
            if (y->mark == false) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
        std::cout << "Cascading cut on node " << y->value << std::endl;
    }

    Node* findNode(int value) {
        return findNodeRecursive(minNode, value);
    }

    Node* findNodeRecursive(Node* node, int value) {
        if (node == nullptr) return nullptr;
        if (node->value == value) return node;
        Node* result = findNodeRecursive(node->child, value);
        if (result != nullptr) return result;
        result = findNodeRecursive(node->right, value);
        return result;
    }
};

class SkewHeap {
public:
    struct Node {
        int key;
        int value;
        Node* left;
        Node* right;
    };

    Node* root;

    SkewHeap() : root(nullptr) {}

    void insert(int node, int distance) {
        Node* newNode = new Node{distance, node, nullptr, nullptr};
        root = merge(root, newNode);
        std::cout << "Inserted node " << node << " with distance " << distance << std::endl;
    }

    int extractMin() {
        if (root == nullptr) {
            throw std::runtime_error("Heap is empty");
        }
        int minKey = root->key;
        int minValue = root->value;
        Node* newRoot = merge(root->left, root->right);
        delete root;
        root = newRoot;
        std::cout << "Extracted min node " << minValue << " with distance " << minKey << std::endl;
        return minValue;
    }

    void decreaseKey(int node, int newDistance) {
        Node* targetNode = findNode(node);
        if (targetNode == nullptr) {
            throw std::runtime_error("Node not found");
        }
        if (newDistance >= targetNode->key) {
            return;
        }
        targetNode->key = newDistance;
        Node* newRoot = merge(root, targetNode);
        delete root;
        root = newRoot;
        std::cout << "Decreased key of node " << node << " to " << newDistance << std::endl;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

private:
    Node* merge(Node* h1, Node* h2) {
        if (h1 == nullptr) return h2;
        if (h2 == nullptr) return h1;
        if (h1->key > h2->key) std::swap(h1, h2);
        h1->right = merge(h2, h1->right);
        std::swap(h1->left, h1->right);
        std::cout << "Merged node " << (h1 ? h1->value : -1) << " and node " << (h2 ? h2->value : -1) << std::endl;
        return h1;
    }

    Node* findNode(int value) {
        return findNodeRecursive(root, value);
    }

    Node* findNodeRecursive(Node* node, int value) {
        if (node == nullptr) return nullptr;
        if (node->value == value) return node;
        Node* result = findNodeRecursive(node->left, value);
        if (result != nullptr) return result;
        result = findNodeRecursive(node->right, value);
        return result;
    }
};

int main() {
    try {
        Graph graph = readGraphFile("USA-road-d.NY.gr");
        std::cout << "Graph loaded successfully" << std::endl;
        std::cout << "Number of nodes: " << graph.adjList.size() << std::endl;

        FibonacciHeap fibHeap;
        SkewHeap skewHeap;

        int startNode = 1;
        int endNode = 2;
        int shortestPathFib = dijkstra(graph, startNode, endNode, fibHeap);
        int shortestPathSkew = dijkstra(graph, startNode, endNode, skewHeap);

        std::cout << "Shortest path from " << startNode << " to " << endNode << " using Fibonacci Heap: " << shortestPathFib << std::endl;
        std::cout << "Shortest path from " << startNode << " to " << endNode << " using Skew Heap: " << shortestPathSkew << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}