#include <iostream>
#include "value.h"
#include <memory>

int main()
{
    auto w1 = make_shared<Value>(2.0, "w1", true);
    auto x1 = make_shared<Value>(3.0, "x1", true);
    auto b1 = make_shared<Value>(6.0, "b1", true);
    auto z1 = w1 * x1;
    z1->label = "z1";
    auto z = z1 + b1;
    z->label = "z";
    auto y = tanh(z);
    y->label = "y";
    y->grad = 1;
    y->print();
    y->backward();
    z->print();
    z->backward();
    z1->print();
    b1->print();
    z1->backward();
    w1->print();
    x1->print();
}
