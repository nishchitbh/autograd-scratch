#include <iostream>
#include "value.h"
#include <memory>

int main()
{
    auto a = make_shared<Value>(2.0, "a", false);
    auto b = make_shared<Value>(3.0, "b", true);
    a->print();
    b->print();
    auto c = a + b;
    c->label = "c";
    auto d = c * a;
    d->print();
}
