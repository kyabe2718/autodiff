#pragma once

namespace AutoDiff::ET
{
namespace detail
{
template <class BinOp>
requires std::is_empty_v<BinOp> struct FoldLeftBinOp
{
    template <class... Args>
    constexpr decltype(auto) operator()(Args&&... args)
    {
        return hana::fold_left(hana::make_tuple(std::forward<Args>(args)...), [](auto&& l, auto&& r) {
            return BinOp{}(std::forward<decltype(l)>(l), std::forward<decltype(r)>(r));
        });
    }
};
}  // namespace detail

template <class F>
struct ETBinOp;

template <class Op, class... Args>
struct ETBinOp<Op(Args...)> : ETFunc<detail::FoldLeftBinOp<Op>(Args...)>
{
    using Base = ETFunc<detail::FoldLeftBinOp<Op>(Args...)>;
    using Base::Base;
};

namespace ADL
{
template <class Left, class Right>
constexpr auto operator+(Left&& l, Right&& r)
{
    return ETBinOp<std::plus<void>(std::decay_t<Left>, std::decay_t<Right>)>{std::forward<Left>(l), std::forward<Right>(r)};
}
template <class Left, class Right>
constexpr auto operator-(Left&& l, Right&& r)
{
    return ETBinOp<std::minus<void>(std::decay_t<Left>, std::decay_t<Right>)>{std::forward<Left>(l), std::forward<Right>(r)};
}
template <class Left, class Right>
constexpr auto operator*(Left&& l, Right&& r)
{
    return ETBinOp<std::multiplies<void>(std::decay_t<Left>, std::decay_t<Right>)>{std::forward<Left>(l), std::forward<Right>(r)};
}
template <class Left, class Right>
constexpr auto operator/(Left&& l, Right&& r)
{
    return ETBinOp<std::divides<void>(std::decay_t<Left>, std::decay_t<Right>)>{std::forward<Left>(l), std::forward<Right>(r)};
}
template <class Left, class Right>
constexpr auto operator%(Left&& l, Right&& r)
{
    return ETBinOp<std::modulus<void>(std::decay_t<Left>, std::decay_t<Right>)>{std::forward<Left>(l), std::forward<Right>(r)};
}
}  // namespace ADL
}  // namespace AutoDiff::ET