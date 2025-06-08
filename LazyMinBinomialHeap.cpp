#include <vector>
#include <cmath>
#include <functional>
#include <algorithm>
#include <string>
#include "LazyMinBinomialHeap.h"
#include "BinomialNode.h"
using namespace std;

bool LazyMinBinomialHeap::_isEmpty(size_t size) {
    return size == 0;
}

BinomialNode* LazyMinBinomialHeap::_enqueue(BinomialNode* head, size_t size, BinomialNode* minRoot, const string& element, const int& priority) {
    BinomialNode* newRoot = new BinomialNode(element, priority);

    if (head) {
        newRoot->setSibling(head);
    }
    head = newRoot;
    size++;

    if (!minRoot || newRoot->getPriority() > minRoot->getPriority()) {
        minRoot = newRoot;
    }

    return newRoot;
}

BinomialNode* LazyMinBinomialHeap::_dequeue(BinomialNode* head, size_t size, int smallestValue) {
    BinomialNode* smallestRoot = removeSmallestRoot();
    if (!smallestRoot) {
        return nullptr;
    }

    size--;

    if (smallestRoot->getChild()) {
        BinomialNode* child = smallestRoot->getChild();
        BinomialNode* lastChild = nullptr;

        while (child) {
            lastChild = child;
            child->setParent(nullptr);
            child = child->getSibling();
        }

        if (head) {
            lastChild->setSibling(head);
        }
        head = smallestRoot->getChild();
    }

    head = consolidate();

    if (minRoot == smallestRoot) {
        recalculateMin();
    }

    return smallestRoot;
}

BinomialNode* LazyMinBinomialHeap::_deleteNode(BinomialNode* node, int smallestValue) {
    decreasePriority(node, smallestValue);
    return dequeue();
}

BinomialNode* LazyMinBinomialHeap::_peek(BinomialNode* minRoot) {
    return minRoot;
}

LazyMinBinomialHeap LazyMinBinomialHeap::_unionHeap(LazyMinBinomialHeap& otherHeap, BinomialNode* head, size_t size, BinomialNode* minRoot) {
    LazyMinBinomialHeap unionedHeap(smallestValue);
    unionedHeap.head = head;

    BinomialNode* cur = unionedHeap.head;
    while (cur && cur->getSibling()) {
        cur = cur->getSibling();
    }

    if (cur) {
        cur->setSibling(otherHeap.head);
    }
    else {
        unionedHeap.head = otherHeap.head;
    }

    unionedHeap.size = size + otherHeap.size;


    otherHeap.head = nullptr;
    otherHeap.size = 0;
    otherHeap.minRoot = nullptr;

    return unionedHeap;
}

void LazyMinBinomialHeap::swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

bool LazyMinBinomialHeap::_decreasePriority(BinomialNode* node, const int& newValue) {
    if (node->getPriority() < newValue) {
        return false;
    }

    node->setPriority(newValue);
    BinomialNode* cur = node;
    BinomialNode* parent = cur->getParent();

    while (parent && cur->getPriority() < parent->getPriority()) {
        swap(cur->getPriority(), parent->getPriority());
        cur = parent;
        parent = cur->getParent();
    }

    return true;
}

void LazyMinBinomialHeap::deleteSubtree(BinomialNode* node) {
    if (!node) return;
    deleteSubtree(node->getSibling());
    deleteSubtree(node->getChild());
    delete node;
}

BinomialNode* LazyMinBinomialHeap::removeSmallestRoot() {
    if (!head) return nullptr;

    BinomialNode* cur = head;
    BinomialNode* prev = cur;
    BinomialNode* min = cur;
    BinomialNode* prevMin = nullptr;
    cur = cur->getSibling();

    while (cur) {
        if (cur->getPriority() > min->getPriority()) {
            min = cur;
            prevMin = prev;
        }

        prev = cur;
        cur = cur->getSibling();
    }

    if (!prevMin) {
        head = head->getSibling();
    }
    else {
        prevMin->setSibling(min->getSibling());
    }

    return min;
}

void LazyMinBinomialHeap::recalculateMin() {
    if (!head) {
        minRoot = nullptr;
        return;
    }

    BinomialNode* cur = head->getSibling();
    minRoot = head;

    while (cur) {
        if (cur->getPriority() > minRoot->getPriority()) {
            minRoot = cur;
        }
        cur = cur->getSibling();
    }
}

BinomialNode* LazyMinBinomialHeap::consolidate() {
    vector<vector<BinomialNode*>> sortedTrees = sortForest();

    for (size_t k = 0; k < sortedTrees.size(); k++) {
        auto& degreeKTrees = sortedTrees[k];
        size_t numberOfDegreeKTrees = degreeKTrees.size();

        while (numberOfDegreeKTrees >= 2) {
            BinomialNode* treeA = degreeKTrees.back();
            degreeKTrees.pop_back();
            BinomialNode* treeB = degreeKTrees.back();
            degreeKTrees.pop_back();

            BinomialNode* linkedTree = treeA->getPriority() > treeB->getPriority()
                ? linkTrees(treeA, treeB)
                : linkTrees(treeB, treeA);

            if (k + 1 >= sortedTrees.size()) {
                sortedTrees.resize(k + 2);
            }
            sortedTrees[k + 1].push_back(linkedTree);

            numberOfDegreeKTrees -= 2;
        }
    }

    BinomialNode* cur = nullptr;
    BinomialNode* newHead = nullptr;

    for (int i = sortedTrees.size() - 1; i >= 0; i--) {
        if (sortedTrees[i].empty()) continue;

        BinomialNode* tree = sortedTrees[i][0];
        if (!cur) {
            cur = tree;
            newHead = cur;
        }
        else {
            cur->setSibling(tree);
            cur = cur->getSibling();
        }
    }

    if (cur) {
        cur->setSibling(nullptr);
    }

    return newHead;
}

BinomialNode* LazyMinBinomialHeap::linkTrees(BinomialNode* treeA, BinomialNode* treeB) {
    treeB->setParent(treeA);
    treeB->setSibling(treeA->getChild());
    treeA->setChild(treeB);
    treeA->setDegree(treeA->getDegree() + 1);
    treeA->setSibling(nullptr);
    return treeA;
}

vector<vector<BinomialNode*>> LazyMinBinomialHeap::sortForest(BinomialNode* head, size_t size) {
    size_t maxDegree = static_cast<size_t>(std::ceil(std::log2(size + 1)));
    vector<vector<BinomialNode*>> sortedTrees(maxDegree + 1);

    BinomialNode* cur = head;
    while (cur) {
        BinomialNode* next = cur->getSibling();
        cur->setSibling(nullptr);

        size_t index = cur->getDegree();
        if (index >= sortedTrees.size()) {
            sortedTrees.resize(index + 1);
        }
        sortedTrees[index].push_back(cur);
        cur = next;
    }

    return sortedTrees;
}
