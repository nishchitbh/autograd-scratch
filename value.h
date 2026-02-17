#pragma once
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <unordered_set>
using namespace std;

class Value : public enable_shared_from_this<Value>
{
private:
    string op;

public:
    double data;
    string label;
    vector<shared_ptr<Value>> prev;
    bool verbose;
    double grad;
    function<void()> _backward;
    Value(double data = 0,
          string label = "",
          bool verbose = false,
          vector<shared_ptr<Value>> parents = {},
          string op = "");
    void print();
    void backward();
};

shared_ptr<Value> operator+(const shared_ptr<Value> &a, const shared_ptr<Value> &b);

shared_ptr<Value> operator*(const shared_ptr<Value> &a, const shared_ptr<Value> &b);

shared_ptr<Value> ReLU(const shared_ptr<Value> &a);
shared_ptr<Value> tanh(const shared_ptr<Value> &a);
void printGraph(const std::shared_ptr<Value> &a);
