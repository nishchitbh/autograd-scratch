#pragma once
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Value
{
private:
    string op;

public:
    double data;
    string label;
    vector<shared_ptr<Value>> prev;
    bool verbose;
    double grad;
    Value(double data = 0,
          string label = "",
          bool verbose = false,
          vector<shared_ptr<Value>> parents = {},
          string op = "");
    void print();
};

shared_ptr<Value> operator+(const shared_ptr<Value> &a, const shared_ptr<Value> &b);

shared_ptr<Value> operator*(const shared_ptr<Value> &a, const shared_ptr<Value> &b);

shared_ptr<Value> ReLU(const shared_ptr<Value> &a);