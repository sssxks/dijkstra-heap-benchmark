#pragma once

#include <vector>
#include <stdexcept>

/**
 * @file heap.hpp
 * @brief Defines the interface for a generic Heap data structure.
 */

/**
 * @class Heap
 * @brief An abstract base class representing a generic heap.
 * 
 * This class provides a common interface for heap operations such as 
 * inserting elements, removing the top element, and checking if the heap is empty.
 * 
 * @tparam T The type of elements stored in the heap.
 */
template <typename T>
class Heap
{
public:
    /**
     * @brief Inserts a new element into the heap.
     * 
     * @param value The element to be inserted.
     */
    virtual void push(const T &value) = 0;

    /**
     * @brief Removes and returns the top element from the heap.
     * 
     * @return The top element of the heap.
     */
    virtual T pop() = 0;

    /**
     * @brief Checks if the heap is empty.
     * 
     * @return true if the heap is empty, false otherwise.
     */
    virtual bool empty() const = 0;

    /**
     * @brief Virtual destructor for the Heap class.
     */
    virtual ~Heap() = default;
};