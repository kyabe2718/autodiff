#undef NDEBUG
#include <cassert>

#include <autodiff/var.hpp>

#include <iostream>

int main()
{
    using namespace AutoDiff;

    Var<0> x;
    Var<1> y;

    constexpr auto r = 1 + x + y;

    constexpr auto v = r.eval(hana::make_tuple(1, 2));

    std::cout << r.eval(hana::make_tuple(1, 2)) << std::endl;
}
