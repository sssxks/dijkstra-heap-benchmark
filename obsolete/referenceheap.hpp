#pragma once

#include "heap.hpp"
#include <queue>
#include <vector>

template <typename T>
class ReferenceHeap : public Heap<T>
{
public:
    ReferenceHeap() : heap() {}
    void push(const T &value) override
    {
        heap.push(value);
    }

    T pop() override
    {
        if (heap.empty())
        {
            throw std::out_of_range("Heap is empty");
        }
        T top = heap.top();
        heap.pop();
        return top;
    }

    bool empty() const override
    {
        return heap.empty();
    }

private:
    std::priority_queue<T, std::vector<T>> heap;
};