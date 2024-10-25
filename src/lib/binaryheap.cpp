#include "heap.hpp"

template <typename T>
class BinaryHeap : public Heap<T>
{
public:
    BinaryHeap() = default;
    void insert(const T &value) override;
    T extractMin() override;
    bool isEmpty() const override;

private:
    std::vector<T> data;
    void heapifyUp(int index);
    void heapifyDown(int index);
};

template <typename T>
void BinaryHeap<T>::insert(const T &value)
{
    data.push_back(value);
    heapifyUp(data.size() - 1);
}

template <typename T>
T BinaryHeap<T>::extractMin()
{
    if (isEmpty())
    {
        throw std::runtime_error("Heap is empty");
    }
    T minValue = data.front();
    data.front() = data.back();
    data.pop_back();
    if (!isEmpty())
    {
        heapifyDown(0);
    }
    return minValue;
}

template <typename T>
bool BinaryHeap<T>::isEmpty() const
{
    return data.empty();
}

template <typename T>
void BinaryHeap<T>::heapifyUp(int index)
{
    while (index > 0)
    {
        int parentIndex = (index - 1) / 2;
        if (data[index] >= data[parentIndex])
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
    int size = data.size();
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