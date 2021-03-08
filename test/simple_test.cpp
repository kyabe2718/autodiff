#include <autodiff/et.hpp>

#include <Eigen/Core>
#include <iostream>

using namespace AutoDiff::ET;

constexpr auto f = [](auto x, auto y) {
    return x + y + 1;
};

int main()
{
    {
        ETVar<0> x;
        ETVar<1> y;

        std::cout << sizeof(hana::make_tuple(x, x)) << std::endl;
        std::cout << sizeof(x) << std::endl;
        std::cout << sizeof(x + x + x + x) << std::endl;
        std::cout << sizeof(x + y) << std::endl;

        constexpr auto r = f(x, y);
        r.apply(hana::make_tuple(1, 2));
    }
}
