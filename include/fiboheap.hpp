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
    FiboHeap(const FiboHeap &other) : minNode(nullptr), nodeCount(0)
    {
        if (other.minNode != nullptr)
        {
            std::unordered_map<Node *, Node *> nodeMap;
            std::vector<Node *> nodesToCopy;

            Node *current = other.minNode;
            do
            {
                nodesToCopy.push_back(current);
                current = current->right;
            } while (current != other.minNode);

            for (Node *node : nodesToCopy)
            {
                Node *newNode = new Node(node->value);
                nodeMap[node] = newNode;
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
                    if (newNode->value < minNode->value)
                    {
                        minNode = newNode;
                    }
                }
            }

            for (Node *node : nodesToCopy)
            {
                Node *newNode = nodeMap[node];
                if (node->child != nullptr)
                {
                    newNode->child = nodeMap[node->child];
                }
                if (node->parent != nullptr)
                {
                    newNode->parent = nodeMap[node->parent];
                }
                if (node->left != nullptr)
                {
                    newNode->left = nodeMap[node->left];
                }
                if (node->right != nullptr)
                {
                    newNode->right = nodeMap[node->right];
                }
                newNode->degree = node->degree;
                newNode->mark = node->mark;
            }

            nodeCount = other.nodeCount;
        }
    }

    /**
     * @brief Copy assignment operator for FiboHeap.
     *
     * @param other The other Fibonacci Heap to assign from.
     * @return A reference to this Fibonacci Heap.
     */
    FiboHeap &operator=(const FiboHeap &other)
    {
        if (this != &other)
        {
            // Clear the current heap
            while (!empty())
            {
                pop();
            }

            // Copy the other heap
            if (other.minNode != nullptr)
            {
                std::unordered_map<Node *, Node *> nodeMap;
                std::vector<Node *> nodesToCopy;

                Node *current = other.minNode;
                do
                {
                    nodesToCopy.push_back(current);
                    current = current->right;
                } while (current != other.minNode);

                for (Node *node : nodesToCopy)
                {
                    Node *newNode = new Node(node->value);
                    nodeMap[node] = newNode;
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
                        if (newNode->value < minNode->value)
                        {
                            minNode = newNode;
                        }
                    }
                }

                for (Node *node : nodesToCopy)
                {
                    Node *newNode = nodeMap[node];
                    if (node->child != nullptr)
                    {
                        newNode->child = nodeMap[node->child];
                    }
                    if (node->parent != nullptr)
                    {
                        newNode->parent = nodeMap[node->parent];
                    }
                    if (node->left != nullptr)
                    {
                        newNode->left = nodeMap[node->left];
                    }
                    if (node->right != nullptr)
                    {
                        newNode->right = nodeMap[node->right];
                    }
                    newNode->degree = node->degree;
                    newNode->mark = node->mark;
                }

                nodeCount = other.nodeCount;
            }
        }
        return *this;
    }

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
    bool empty() const override
    {
        return minNode == nullptr;
    }

    /**
     * @brief Destructor for FiboHeap.
     */
    ~FiboHeap()
    {
        if (minNode != nullptr)
        {
            std::vector<Node *> nodesToDelete;
            Node *current = minNode;
            do
            {
                nodesToDelete.push_back(current);
                current = current->right;
            } while (current != minNode);

            for (Node *node : nodesToDelete)
            {
                std::vector<Node *> childNodes;
                if (node->child != nullptr)
                {
                    Node *child = node->child;
                    do
                    {
                        childNodes.push_back(child);
                        child = child->right;
                    } while (child != node->child);
                }

                for (Node *child : childNodes)
                {
                    delete child;
                }

                delete node;
            }
        }
    }
};

template <typename T>
/**
 * @brief Links two nodes in the heap.
 *
 * This function links node y to node x, making y a child of x.
 *
 * @param y The node to be linked.
 * @param x The node to which y will be linked.
 */
void FiboHeap<T>::link(Node *y, Node *x)
{
    // Remove y from the root list
    y->left->right = y->right;
    y->right->left = y->left;

    // Make y a child of x
    y->parent = x;
    if (x->child == nullptr)
    {
        // If x has no children, make y its only child
        x->child = y;
        y->right = y;
        y->left = y;
    }
    else
    {
        // Otherwise, add y to the child list of x
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    // Increase the degree of x
    x->degree++;

    // Mark y as unmarked
    y->mark = false;
}

template <typename T>
void FiboHeap<T>::consolidate()
{
    // Calculate the maximum degree of any node in the heap
    int maxDegree = static_cast<int>(std::log2(nodeCount)) + 1;
    // Create an array to store roots of trees with different degrees
    std::vector<Node *> A(maxDegree, nullptr);

    // Create a list of root nodes
    std::vector<Node *> rootList;
    Node *x = minNode;
    if (x != nullptr)
    {
        // Traverse the root list and add nodes to rootList
        do
        {
            rootList.push_back(x);
            x = x->right;
        } while (x != minNode);
    }

    // Process each node in the root list
    for (Node *w : rootList)
    {
        x = w;
        int d = x->degree;
        // Merge trees of the same degree
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

    // Reconstruct the root list from the array A
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
inline FiboHeap<T>::FiboHeap() : minNode(nullptr), nodeCount(0)
{
}

template <typename T>
void FiboHeap<T>::push(const T &value)
{
    // Create a new node with the given value
    Node *newNode = new Node(value);

    // If the heap is empty, set the new node as the minimum node
    if (minNode == nullptr)
    {
        minNode = newNode;
    }
    else
    {
        // Insert the new node into the root list
        newNode->left = minNode;
        newNode->right = minNode->right;
        minNode->right->left = newNode;
        minNode->right = newNode;

        // Update the minimum node if necessary
        if (value < minNode->value)
        {
            minNode = newNode;
        }
    }

    // Increment the node count
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

    // If the minimum node has children, add them to the root list
    if (z->child != nullptr)
    {
        Node *x = z->child;
        do
        {
            Node *next = x->right;

            // Remove x from the child list
            x->left->right = x->right;
            x->right->left = x->left;

            // Add x to the root list
            x->left = minNode;
            x->right = minNode->right;
            minNode->right->left = x;
            minNode->right = x;

            // Set x's parent to nullptr
            x->parent = nullptr;

            x = next;
        } while (x != z->child);
    }

    // Remove z from the root list
    z->left->right = z->right;
    z->right->left = z->left;

    // If z was the only node in the root list, set minNode to nullptr
    if (z == z->right)
    {
        minNode = nullptr;
    }
    else
    {
        // Otherwise, set minNode to the next node and consolidate the heap
        minNode = z->right;
        consolidate();
    }

    T minValue = z->value;
    delete z;
    nodeCount--;

    return minValue;
}
