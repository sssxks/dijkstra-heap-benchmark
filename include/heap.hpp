#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class Heap {
public:
    virtual void insert(const T& value) = 0;
    virtual T extractMin() = 0;
    virtual bool isEmpty() const = 0;
    virtual ~Heap() = default;
};