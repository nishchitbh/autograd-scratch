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

        cout << "})" << endl;
    }
    else
    {
        cout << "Value(" << data << ")" << endl;
    }
}

shared_ptr<Value> operator+(const shared_ptr<Value> &a, const shared_ptr<Value> &b)
{
    return make_shared<Value>(a->data + b->data,
                              a->label + "+" + b->label,
                              a->verbose || b->verbose,
                              vector<shared_ptr<Value>>{a, b},
                              "+");
}

shared_ptr<Value> operator*(const shared_ptr<Value> &a, const shared_ptr<Value> &b)
{
    return make_shared<Value>(a->data * b->data,
                              a->label + "*" + b->label,
                              a->verbose || b->verbose,
                              vector<shared_ptr<Value>>{a, b},
                              "*");
}
