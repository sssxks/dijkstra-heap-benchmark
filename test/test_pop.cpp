#include <gtest/gtest.h>
#include "referenceheap.hpp"

// Test popping from an empty heap should throw an exception
TEST(ReferenceHeapTest, PopEmptyHeapThrowsException)
{
    ReferenceHeap<int> heap;
    EXPECT_THROW(heap.pop(), std::out_of_range);
}

// Test popping from a heap with one element should return that element
TEST(ReferenceHeapTest, PopSingleElement)
{
    ReferenceHeap<int> heap;
    heap.push(42);
    EXPECT_EQ(heap.pop(), 42);
    EXPECT_TRUE(heap.empty());
}

// Test popping from a heap with multiple elements should return elements in the correct order
TEST(ReferenceHeapTest, PopMultipleElements)
{
    ReferenceHeap<int> heap;
    heap.push(10);
    heap.push(20);
    heap.push(5);

    EXPECT_EQ(heap.pop(), 20);
    EXPECT_EQ(heap.pop(), 10);
    EXPECT_EQ(heap.pop(), 5);
    EXPECT_TRUE(heap.empty());
}