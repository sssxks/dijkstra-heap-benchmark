#include "heap.hpp"

template <typename T>
/**
 * @class BinaryHeap
 * @brief A binary heap implementation of a heap data structure.
 * 
 * This class provides a binary heap which supports typical heap operations
 * such as push, pop, and checking if the heap is empty. It inherits from
 * the Heap<T> interface.
 * 
 * @tparam T The type of elements stored in the heap.
 */
class BinaryHeap : public Heap<T>
{
public:
    BinaryHeap() : data() {}
    void push(const T &value) override;
    T pop() override;
    bool empty() const override;

private:
    std::vector<T> data;
    void heapifyUp(int index);
    void heapifyDown(int index);
};

template <typename T>
void BinaryHeap<T>::push(const T &value)
{
    data.push_back(value);
    heapifyUp(static_cast<int>(data.size() - 1));
}

template <typename T>
T BinaryHeap<T>::pop()
{
    if (empty())
    {
        throw std::runtime_error("Heap is empty");
    }
    T minValue = data.front();
    data.front() = data.back();
    data.pop_back();
    if (!empty())
    {
        heapifyDown(0);
    }
    return minValue;
}

template <typename T>
bool BinaryHeap<T>::empty() const
{
    return data.empty();
}

template <typename T>
void BinaryHeap<T>::heapifyUp(int index)
{
    while (index > 0)
    {
        int parentIndex = (index - 1) / 2;
        if (data[index] > data[parentIndex])
        {
            break;
        }
        std::swap(data[index], data[parentIndex]);
        index = parentIndex;
    }
}

template <typename T>
void BinaryHeap<T>::heapifyDown(int index)
{
    auto size = static_cast<int>(data.size());
    while (index < size)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < size && data[leftChild] < data[smallest])
        {
            smallest = leftChild;
        }
        if (rightChild < size && data[rightChild] < data[smallest])
        {
            smallest = rightChild;
        }
        if (smallest == index)
        {
            break;
        }
        std::swap(data[index], data[smallest]);
        index = smallest;
    }
}