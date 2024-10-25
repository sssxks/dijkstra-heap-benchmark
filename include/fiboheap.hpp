#pragma once

#include <unordered_map>
#include <cmath>
#include <limits>

#include "heap.hpp"

template <typename T>
class FiboHeap : public Heap<T>
{
private:
    struct Node
    {
        T value;
        Node *parent;
        Node *child;
        Node *left;
        Node *right;
        int degree;
        bool mark;

        Node(const T &val) : value(val), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false) {};
    };

    Node *minNode;
    int nodeCount;

    void link(Node *y, Node *x);
    void consolidate();

public:
    FiboHeap() : minNode(nullptr), nodeCount(0) {};

    void insert(const T &value) override;
    T extractMin() override;
    bool isEmpty() const override;

    ~FiboHeap()
    {
        // Destructor implementation to free allocated memory
        // This part is left as an exercise for the reader
    }
};