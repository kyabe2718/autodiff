#include <vessel/tuple.hpp>

#include <iostream>
#include <tuple>

using namespace vessel;

struct Empty
{};

int main()
{
    auto tp = make_tuple(Empty(), Empty(), Empty());
    std::cout << sizeof(tp) << std::endl;
}