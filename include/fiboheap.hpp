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

    void push(const T &value) override;
    T pop() override;
    bool empty() const override;

    ~FiboHeap()
    {

    }
};