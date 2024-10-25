/**
 * @file fiboheap.hpp
 * @brief Implementation of a Fibonacci Heap.
 *
 * This file contains the implementation of a Fibonacci Heap, which is a data structure
 * consisting of a collection of trees satisfying the minimum-heap property. It supports
 * a variety of operations such as insertion, minimum extraction, and merging of heaps
 * efficiently.
 *
 * @tparam T The type of elements stored in the heap.
 */

#pragma once

#include <unordered_map>
#include <cmath>
#include <limits>

#include "heap.hpp"

/**
 * @class FiboHeap
 * @brief A Fibonacci Heap implementation.
 *
 * This class implements a Fibonacci Heap, which supports efficient heap operations.
 *
 * @tparam T The type of elements stored in the heap.
 */
template <typename T>
class FiboHeap : public Heap<T>
{
private:
    /**
     * @struct Node
     * @brief A node in the Fibonacci Heap.
     *
     * This structure represents a node in the Fibonacci Heap.
     */
    struct Node
    {
        T value;      ///< The value stored in the node.
        Node *parent; ///< Pointer to the parent node.
        Node *child;  ///< Pointer to the child node.
        Node *left;   ///< Pointer to the left sibling node.
        Node *right;  ///< Pointer to the right sibling node.
        int degree;   ///< The degree of the node.
        bool mark;    ///< Mark indicating whether the node has lost a child since it became a child of its current parent.

        /**
         * @brief Constructor for Node.
         *
         * @param val The value to be stored in the node.
         */
        Node(const T &val) : value(val), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false) {};
    };

    Node *minNode; ///< Pointer to the node with the minimum value.
    int nodeCount; ///< The number of nodes in the heap.

    /**
     * @brief Links two nodes in the heap.
     *
     * This function links node y to node x, making y a child of x.
     *
     * @param y The node to be linked.
     * @param x The node to which y will be linked.
     */
    void link(Node *y, Node *x);

    /**
     * @brief Consolidates the heap.
     *
     * This function consolidates the heap by merging trees of the same degree.
     */
    void consolidate();

public:
    /**
     * @brief Default constructor for FiboHeap.
     */
    FiboHeap();

    /**
     * @brief Copy constructor for FiboHeap.
     *
     * @param other The other Fibonacci Heap to copy from.
     */
    FiboHeap(const FiboHeap &other);

    /**
     * @brief Copy assignment operator for FiboHeap.
     *
     * @param other The other Fibonacci Heap to assign from.
     * @return A reference to this Fibonacci Heap.
     */
    FiboHeap &operator=(const FiboHeap &other);

    /**
     * @brief Inserts a value into the heap.
     *
     * @param value The value to be inserted.
     */
    void push(const T &value) override;

    /**
     * @brief Extracts the minimum value from the heap.
     *
     * @return The minimum value in the heap.
     * @throws std::runtime_error If the heap is empty.
     */
    T pop() override;

    /**
     * @brief Checks if the heap is empty.
     *
     * @return True if the heap is empty, false otherwise.
     */
    bool empty() const override;

    /**
     * @brief Destructor for FiboHeap.
     */
    ~FiboHeap();
};

template <typename T>
void FiboHeap<T>::link(Node *y, Node *x)
{
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (x->child == nullptr)
    {
        x->child = y;
        y->right = y;
        y->left = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = false;
}

template <typename T>
void FiboHeap<T>::consolidate()
{
    int maxDegree = static_cast<int>(std::log2(nodeCount)) + 1;
    std::vector<Node *> A(maxDegree, nullptr);

    std::vector<Node *> rootList;
    Node *x = minNode;
    if (x != nullptr)
    {
        do
        {
            rootList.push_back(x);
            x = x->right;
        } while (x != minNode);
    }

    for (Node *w : rootList)
    {
        x = w;
        int d = x->degree;
        while (A[d] != nullptr)
        {
            Node *y = A[d];
            if (x->value > y->value)
            {
                std::swap(x, y);
            }
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;
    for (Node *y : A)
    {
        if (y != nullptr)
        {
            if (minNode == nullptr)
            {
                minNode = y;
                minNode->left = minNode;
                minNode->right = minNode;
            }
            else
            {
                y->left = minNode;
                y->right = minNode->right;
                minNode->right->left = y;
                minNode->right = y;
                if (y->value < minNode->value)
                {
                    minNode = y;
                }
            }
        }
    }
}

template <typename T>
void FiboHeap<T>::push(const T &value)
{
    Node *newNode = new Node(value);
    if (minNode == nullptr)
    {
        minNode = newNode;
    }
    else
    {
        newNode->left = minNode;
        newNode->right = minNode->right;
        minNode->right->left = newNode;
        minNode->right = newNode;
        if (value < minNode->value)
        {
            minNode = newNode;
        }
    }
    nodeCount++;
}

template <typename T>
T FiboHeap<T>::pop()
{
    if (minNode == nullptr)
    {
        throw std::runtime_error("Heap is empty");
    }

    Node *z = minNode;
    if (z->child != nullptr)
    {
        Node *x = z->child;
        do
        {
            Node *next = x->right;
            x->left->right = x->right;
            x->right->left = x->left;
            x->left = minNode;
            x->right = minNode->right;
            minNode->right->left = x;
            minNode->right = x;
            x->parent = nullptr;
            x = next;
        } while (x != z->child);
    }

    z->left->right = z->right;
    z->right->left = z->left;
    if (z == z->right)
    {
        minNode = nullptr;
    }
    else
    {
        minNode = z->right;
        consolidate();
    }

    T minValue = z->value;
    delete z;
    nodeCount--;
    return minValue;
}

template <typename T>
bool FiboHeap<T>::empty() const
{
    return minNode == nullptr;
}