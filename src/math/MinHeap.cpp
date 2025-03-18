#include "math/MinHeap.hpp"

MinHeap::MinHeap() {}

void MinHeap::push(std::pair<float, int> element) {
    heap.push_back(element);
    position[element.second] = heap.size() - 1;
    heapifyUp(heap.size() - 1);
}

std::pair<float, int> MinHeap::top() {
    if (heap.empty()) throw std::runtime_error("Heap is empty");
    return heap[0];
}

void MinHeap::pop() {
    if (heap.empty()) throw std::runtime_error("Heap is empty");
    position.erase(heap[0].second);
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) heapifyDown(0);
}

void MinHeap::remove(int edgeIdx) {
        auto it = position.find(edgeIdx);
        if (it == position.end()) return; // element not found

        int index = it->second;
        position.erase(edgeIdx);

        if (index == heap.size() - 1) {
            heap.pop_back();
        } else {
            heap[index] = heap.back();
            position[heap[index].second] = index;
            heap.pop_back();
            heapifyUp(index);
            heapifyDown(index);
        }
    }

void MinHeap::modify(int targetSecond, float newFirst) {
    auto it = position.find(targetSecond);
    if (it == position.end()) return;

    int index = it->second;
    heap[index].first = newFirst;
    heapifyUp(index);
    heapifyDown(index);
}

bool MinHeap::empty() const {
    return heap.empty();
}

bool MinHeap::find(int targetSecond, std::pair<float, int>& result) const {
    auto it = position.find(targetSecond);
    if (it == position.end()) return false;

    result = heap[it->second];
    return true;
}

int MinHeap::size() const {
    return heap.size();
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].first >= heap[parent].first) break;
        std::swap(heap[index], heap[parent]);
        position[heap[index].second] = index;
        position[heap[parent].second] = parent;
        index = parent;
    }
}

void MinHeap::heapifyDown(int index) {
    int size = heap.size();
    while (index < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && heap[left].first < heap[smallest].first) {
            smallest = left;
        }
        if (right < size && heap[right].first < heap[smallest].first) {
            smallest = right;
        }
        if (smallest == index) break;

        std::swap(heap[index], heap[smallest]);
        position[heap[index].second] = index;
        position[heap[smallest].second] = smallest;
        index = smallest;
    }
}
