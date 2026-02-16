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
    this->backward = []() {};
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
    out->backward = [a, b, out]()
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
    out->backward = [a, b, out]()
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
    out->backward = [a, result, out]()
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
    out->backward = [a, t, out]()
    {
        a->grad += (1 - (t * t)) * out->grad;
    };
    return out;
}

// void printGraph(const shared_ptr<Value> &a)
// {

//     if ((a->prev).size() < 1)
//     {
//         return;
//     }
//     cout << a->label << " | data=" << a->data << " | grad=" << a->grad;
//     printGraph((a->prev)[0]);
// }