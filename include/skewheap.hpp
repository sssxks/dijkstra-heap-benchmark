#pragma once

#include <stdexcept>

#include "heap.hpp"

template <typename T>
class SkewHeap : public Heap<T>
{
private:
    struct Node
    {
        T value;
        Node *left;
        Node *right;

        Node(const T &val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node *root;

    Node* merge(Node *h1, Node *h2)
    {
        if (!h1) return h2;
        if (!h2) return h1;
        if (h1->value > h2->value) std::swap(h1, h2);
        h1->right = merge(h1->right, h2);
        std::swap(h1->left, h1->right);
        return h1;
    }

    void deleteTree(Node *node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    SkewHeap() : root(nullptr) {}

    ~SkewHeap()
    {
        deleteTree(root);
    }

    void push(const T &value) override
    {
        Node *newNode = new Node(value);
        root = merge(root, newNode);
    }

    T pop() override
    {
        if (empty())
        {
            throw std::runtime_error("Heap is empty");
        }
        T minValue = root->value;
        Node *oldRoot = root;
        root = merge(root->left, root->right);
        delete oldRoot;
        return minValue;
    }

    bool empty() const override
    {
        return root == nullptr;
    }
};