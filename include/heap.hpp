#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class Heap
{
public:
    virtual void push(const T &value) = 0;
    virtual T pop() = 0;
    virtual bool empty() const = 0;
    virtual ~Heap() = default;
};