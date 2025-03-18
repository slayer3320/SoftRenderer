#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <stdexcept>

class MinHeap 
{
public:
    MinHeap();

    void push(std::pair<float, int> element);
    std::pair<float, int> top();
    void pop();
    void remove(int edgeIdx);
    void modify(int targetSecond, float newFirst);
    bool empty() const;
    bool find(int targetSecond, std::pair<float, int>& result) const;
    int size() const;

private:
    std::vector<std::pair<float, int>> heap;
    std::unordered_map<int, int> position;

    void heapifyUp(int index);
    void heapifyDown(int index);
};