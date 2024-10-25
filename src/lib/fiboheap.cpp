#include "fiboheap.hpp"

template <typename T>
void FiboHeap<T>::link(Node* y, Node* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (x->child == nullptr) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = false;
}

template <typename T>
void FiboHeap<T>::consolidate() {
    int maxDegree = static_cast<int>(std::log2(nodeCount)) + 1;
    std::vector<Node*> A(maxDegree, nullptr);

    std::vector<Node*> rootList;
    Node* x = minNode;
    if (x != nullptr) {
        do {
            rootList.push_back(x);
            x = x->right;
        } while (x != minNode);
    }

    for (Node* w : rootList) {
        x = w;
        int d = x->degree;
        while (A[d] != nullptr) {
            Node* y = A[d];
            if (x->value > y->value) {
                std::swap(x, y);
            }
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;
    for (Node* y : A) {
        if (y != nullptr) {
            if (minNode == nullptr) {
                minNode = y;
                minNode->left = minNode;
                minNode->right = minNode;
            } else {
                y->left = minNode;
                y->right = minNode->right;
                minNode->right->left = y;
                minNode->right = y;
                if (y->value < minNode->value) {
                    minNode = y;
                }
            }
        }
    }
}

template <typename T>
void FiboHeap<T>::insert(const T& value) {
    Node* newNode = new Node(value);
    if (minNode == nullptr) {
        minNode = newNode;
    } else {
        newNode->left = minNode;
        newNode->right = minNode->right;
        minNode->right->left = newNode;
        minNode->right = newNode;
        if (value < minNode->value) {
            minNode = newNode;
        }
    }
    nodeCount++;
}

template <typename T>
T FiboHeap<T>::extractMin() {
    if (minNode == nullptr) {
        throw std::runtime_error("Heap is empty");
    }

    Node* z = minNode;
    if (z->child != nullptr) {
        Node* x = z->child;
        do {
            Node* next = x->right;
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
    if (z == z->right) {
        minNode = nullptr;
    } else {
        minNode = z->right;
        consolidate();
    }

    T minValue = z->value;
    delete z;
    nodeCount--;
    return minValue;
}