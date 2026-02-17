#include "value.h"

Value::Value(double data,
             string label,
             bool verbose,
             vector<shared_ptr<Value>> parents,
             string op)
{
    this->data = data;
    this->prev = parents;
    this->op = op;
    this->grad = 0;
    this->label = label;
    this->verbose = verbose;
    this->_backward = []() {};
}

void Value::backward()
{
    unordered_set<Value *> visited;
    vector<shared_ptr<Value>> topo;
    function<void(Value *)> dfs = [&](Value *node)
    {
        if (visited.count(node))
            return;
        visited.insert(node);
        for (auto &parent : node->prev)
        {
            dfs(parent.get());
        }
        topo.push_back(node->shared_from_this());
    };
    dfs(this);
    for (auto &node : topo)
        node->grad = 0.0;
    this->grad = 1.0;
    for (auto it = topo.rbegin(); it != topo.rend(); it++)
        if ((*it)->_backward)
            (*it)->_backward();
}

void Value::print()
{
    if (verbose)
    {
        cout << "Value(data=" << data
             << ", label=" << label
             << ", parents={";
        for (size_t i = 0; i < prev.size(); i++)
        {
            cout << prev[i]->label;
            if (i + 1 < prev.size())
                cout << ", ";
        }
        cout << "}, op=" << op << ", grad=" << grad << ")" << endl;
    }
    else
    {
        cout << "Value(" << data << ")" << endl;
    }
}

shared_ptr<Value> operator+(const shared_ptr<Value> &a, const shared_ptr<Value> &b)
{
    auto out = make_shared<Value>(a->data + b->data,
                                  a->label + "+" + b->label,
                                  a->verbose || b->verbose,
                                  vector<shared_ptr<Value>>{a, b},
                                  "+");
    out->_backward = [a, b, out]()
    {
        a->grad += 1.0 * out->grad;
        b->grad += 1.0 * out->grad;
    };
    return out;
}

shared_ptr<Value> operator*(const shared_ptr<Value> &a, const shared_ptr<Value> &b)
{
    auto out = make_shared<Value>(a->data * b->data,
                                  a->label + "*" + b->label,
                                  a->verbose || b->verbose,
                                  vector<shared_ptr<Value>>{a, b},
                                  "*");
    out->_backward = [a, b, out]()
    {
        a->grad += b->data * out->grad;
        b->grad += a->data * out->grad;
    };
    return out;
}

shared_ptr<Value> ReLU(const shared_ptr<Value> &a)
{
    double result = 0;
    if (a->data)
        result = a->data;

    auto out = make_shared<Value>(result, "ReLU(" + a->label + ")", a->verbose, vector<shared_ptr<Value>>{a}, "ReLU");
    out->_backward = [a, result, out]()
    {
        a->grad += (result > 0) * out->grad;
    };
    return out;
}

shared_ptr<Value> tanh(const shared_ptr<Value> &a)
{
    double e = exp(1.0);
    double x = a->data;
    double t = (pow(e, 2 * x) - 1) / (pow(e, 2 * x) + 1);
    auto out = make_shared<Value>(t, "tanh(" + a->label + ")", a->verbose, vector<shared_ptr<Value>>{a}, "tanh");
    out->_backward = [a, t, out]()
    {
        a->grad += (1 - (t * t)) * out->grad;
    };
    return out;
}

void printGraph(
    const shared_ptr<Value> &node,
    const string &prefix,
    bool isLast,
    unordered_set<Value *> &visited)
{
    if (!node || visited.count(node.get()))
        return;
    visited.insert(node.get());
    cout << prefix
         << (isLast ? "+-- " : "|-- ");
    node->print();
    cout << "\n";
    string newPrefix = prefix + (isLast ? "    " : "|   ");
    for (size_t i = 0; i < node->prev.size(); ++i)
    {
        bool lastChild = (i == node->prev.size() - 1);
        printGraph(node->prev[i], newPrefix, lastChild, visited);
    }
}

void printGraph(const shared_ptr<Value> &a)
{
    unordered_set<Value *> visited;
    printGraph(a, "", true, visited);
}