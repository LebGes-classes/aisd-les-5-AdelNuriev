#pragma once
#include "BinomialNode.h"
#include <vector>
class LazyMinBinomialHeap
{

    BinomialNode* head;
    BinomialNode* minRoot;
    size_t size;
    int smallestValue;

public:
    LazyMinBinomialHeap(const int& smallValue)
    {
        head = nullptr;
        minRoot = nullptr;
        size = 0;
        smallestValue = smallValue;
    }

    ~LazyMinBinomialHeap() {
        BinomialNode* current = head;
        while (current) {
            BinomialNode* next = current->getSibling();
            deleteSubtree(current);
            current = next;
        }
    }

    bool isEmpty() { return _isEmpty(size); }

    BinomialNode* enqueue(const string&element, const int& priority) { return _enqueue(head, size, minRoot, element, priority); }

    BinomialNode* dequeue() { return _dequeue(head, size, smallestValue); }

    BinomialNode* deleteNode(BinomialNode* node) { return _deleteNode(node, smallestValue); }

    BinomialNode* peek() { return _peek(minRoot); }

    LazyMinBinomialHeap unionHeap(LazyMinBinomialHeap& otherHeap) { return _unionHeap(otherHeap, head, size, minRoot); }

    bool decreasePriority(BinomialNode* node, const int& newValue) { return _decreasePriority(node, newValue); }

private:
    bool _isEmpty(size_t size);

    BinomialNode* _enqueue(BinomialNode* head, size_t size, BinomialNode* minRoot, const string& element, const int& priority);

    BinomialNode* _dequeue(BinomialNode* head, size_t size, int smallestValue);

    BinomialNode* _deleteNode(BinomialNode* node, int smallestValue);

    BinomialNode* _peek(BinomialNode* minRoot);

    LazyMinBinomialHeap _unionHeap(LazyMinBinomialHeap& otherHeap, BinomialNode* head, size_t size, BinomialNode* minRoot);

    bool _decreasePriority(BinomialNode* node, const int& newValue);

    void swap(int a, int b);

    void deleteSubtree(BinomialNode* node);

    BinomialNode* removeSmallestRoot();

    void recalculateMin();

    BinomialNode* consolidate();

    BinomialNode* linkTrees(BinomialNode* treeA, BinomialNode* treeB);

    vector<vector<BinomialNode*>> sortForest(BinomialNode* head, size_t size);
};

