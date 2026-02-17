#include <iostream>
#include "value.h"
#include <memory>

int main()
{
    auto w1 = make_shared<Value>(2.0, "w1", true);
    auto x1 = make_shared<Value>(3.0, "x1", true);
    auto b1 = make_shared<Value>(18.0, "b1", true);
    auto z1 = w1 * x1;
    z1->label = "z1";
    auto z = z1 * b1;
    z->label = "z";
    auto y = ReLU(z);
    y->label = "y";
    printGraph(y);
    cout << endl
         << endl;
    y->backward();
    printGraph(y);
}