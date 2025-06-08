#pragma once

class BinomialNode
{
private:
    string value;
    int priority;
    int degree;

    BinomialNode* parent;
    BinomialNode* sibling;
    BinomialNode* child;

public:
    BinomialNode(const string& val, const int& prio)
    {
        value = val;
        priority = prio;
        degree = 0;
        parent = nullptr;
        sibling = nullptr;
        child = nullptr;
    }
    ~BinomialNode()
    {
    }

    string getValue() { return value; }
    void setValue(string val) { value = val; }

    int getPriority() { return priority; }
    void setPriority(int value) { priority = value; }

    int getDegree() { return degree; }
    void setDegree(int value) { degree = value; }

    BinomialNode* getParent() { return parent; }
    void setParent(BinomialNode* par) { parent = par; }

    BinomialNode* getSibling() { return sibling; }
    void setSibling(BinomialNode* sib) { sibling = sib; }

    BinomialNode* getChild() { return child; }
    void setChild(BinomialNode* chi) { child = chi; }
};

