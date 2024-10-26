#pragma once

#include "heap.hpp"
#include <stdexcept>

/**
 * @file skewheap.hpp
 * @brief Implementation of a Skew Heap data structure.
 *
 * This file contains the definition and implementation of a Skew Heap,
 * which is a variant of a binary heap that supports efficient merging
 * of two heaps.
 *
 * @tparam T The type of elements stored in the heap.
 */

/**
 * @class SkewHeap
 * @brief A Skew Heap implementation.
 *
 * The SkewHeap class provides methods to insert elements, remove the
 * minimum element, and check if the heap is empty. It supports efficient
 * merging of two heaps.
 *
 * @tparam T The type of elements stored in the heap.
 */
template <typename T>
class SkewHeap : public Heap<T>
{
private:
    /**
     * @struct Node
     * @brief A node in the Skew Heap.
     *
     * The Node struct represents a single node in the Skew Heap, containing
     * a value and pointers to its left and right children.
     */
    struct Node
    {
        T value;     ///< The value stored in the node.
        Node *left;  ///< Pointer to the left child.
        Node *right; ///< Pointer to the right child.

        /**
         * @brief Constructs a new Node.
         *
         * @param val The value to be stored in the node.
         */
        Node(const T &val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node *root; ///< Pointer to the root of the Skew Heap.

    /**
     * @brief Merges two Skew Heaps.
     *
     * This function merges two Skew Heaps and returns the root of the
     * resulting heap.
     *
     * @param h1 Pointer to the root of the first heap.
     * @param h2 Pointer to the root of the second heap.
     * @return Node* Pointer to the root of the merged heap.
     */
    Node *merge(Node *h1, Node *h2)
    {
        if (!h1)
            return h2;
        if (!h2)
            return h1;
        if (h1->value > h2->value)
            std::swap(h1, h2);
        h1->right = merge(h1->right, h2);
        std::swap(h1->left, h1->right);
        return h1;
    }

    /**
     * @brief Deletes the entire tree.
     *
     * This function deletes all nodes in the tree starting from the given node.
     *
     * @param node Pointer to the root of the tree to be deleted.
     */
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
    /**
     * @brief Copy constructor.
     *
     * This constructor creates a deep copy of the given SkewHeap.
     *
     * @param other The SkewHeap to copy.
     */
    SkewHeap(const SkewHeap &other) : root(nullptr)
    {
        if (other.root)
        {
            root = new Node(other.root->value);
            copyTree(root, other.root);
        }
    }

    /**
     * @brief Copy assignment operator.
     *
     * This operator assigns a deep copy of the given SkewHeap to this SkewHeap.
     *
     * @param other The SkewHeap to copy.
     * @return SkewHeap& A reference to this SkewHeap.
     */
    SkewHeap &operator=(const SkewHeap &other)
    {
        if (this != &other)
        {
            deleteTree(root);
            root = nullptr;
            if (other.root)
            {
                root = new Node(other.root->value);
                copyTree(root, other.root);
            }
        }
        return *this;
    }
    /**
     * @brief Constructs a new SkewHeap.
     */
    SkewHeap() : root(nullptr) {}

    /**
     * @brief Destroys the SkewHeap.
     *
     * This destructor deletes all nodes in the heap.
     */
    ~SkewHeap()
    {
        deleteTree(root);
    }

    /**
     * @brief Inserts a new element into the heap.
     *
     * This function inserts a new element into the heap by merging the
     * new node with the existing heap.
     *
     * @param value The value to be inserted.
     */
    void push(const T &value) override
    {
        Node *newNode = new Node(value);
        root = merge(root, newNode);
    }

    /**
     * @brief Removes and returns the minimum element from the heap.
     *
     * This function removes the minimum element from the heap and returns
     * its value. If the heap is empty, it throws a runtime error.
     *
     * @return T The minimum element in the heap.
     * @throws std::runtime_error If the heap is empty.
     */
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

    /**
     * @brief Checks if the heap is empty.
     *
     * This function returns true if the heap is empty, and false otherwise.
     *
     * @return bool True if the heap is empty, false otherwise.
     */
    bool empty() const override
    {
        return root == nullptr;
    }

private:
    /**
     * @brief Copies the tree from the source node to the destination node.
     *
     * This function performs a deep copy of the tree starting from the source node
     * to the destination node.
     *
     * @param dest The destination node.
     * @param src The source node.
     */
    void copyTree(Node *&dest, Node *src)
    {
        if (src)
        {
            if (src->left)
            {
                dest->left = new Node(src->left->value);
                copyTree(dest->left, src->left);
            }
            if (src->right)
            {
                dest->right = new Node(src->right->value);
                copyTree(dest->right, src->right);
            }
        }
    }
};