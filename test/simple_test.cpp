#include <autodiff/et.hpp>

#include <Eigen/Core>
#include <iostream>

using namespace AutoDiff::ET;

constexpr auto f = [](auto x, auto y) {
    return x + y + 1;
};

template<class T>
struct Hoge
{
    Hoge() requires(std::is_trivially_constructible_v<T>) = default;
};

int main()
{
    std::cout << std::boolalpha << std::is_trivially_constructible_v<Hoge<int>> << std::endl;

    //    ETVar<0> x;
    //    ETVar<1> y;
    //
    //    std::cout << std::boolalpha << vessel::detail::is_empty_elm<decltype(x + x)> << std::endl;
    //    std::cout << std::boolalpha << vessel::detail::is_empty_elm<int> << std::endl;
    //
    //
    //    std::cout << std::boolalpha << std::is_empty_v<decltype(x + x)> << std::endl;
    //    std::cout << std::boolalpha << std::is_trivially_constructible_v<decltype(x + x)> << std::endl;
    //    std::cout << std::boolalpha << std::is_trivially_copyable_v<decltype(x + x)> << std::endl;
    //    std::cout << std::boolalpha << std::is_trivially_destructible_v<decltype(x + x)> << std::endl;
    //
    //    std::cout << sizeof(x + x) << std::endl;
    //    std::cout << sizeof(x + x + x) << std::endl;
    //
    //
    //    constexpr auto r = f(x, y);
    //    r.apply(hana::make_tuple(1, 2));

    return 0;
}
