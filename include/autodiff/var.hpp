#pragma once

#include <tuple>
#include <boost/hana.hpp>

#include "ebo.hpp"

namespace AutoDiff
{
namespace hana = boost::hana;

namespace ADL
{
struct VarBase
{
    template <class V, class... Params>
    constexpr static auto evalImpl(V& v, const hana::tuple<Params...>& p)
    {
        if constexpr (std::is_base_of_v<VarBase, V>) {
            return v.eval(p);
        } else {
            return v;
        }
    }
};
}  // namespace ADL

using VarBase = ADL::VarBase;

template <class... T>
struct ExprBase;

template <std::size_t... I, class... Args>
struct ExprBase<std::index_sequence<I...>, Args...>
    : private ebo<Args, std::integral_constant<std::size_t, I>>...
{
    constexpr explicit ExprBase(Args... args)
        : ebo<Args, std::integral_constant<std::size_t, I>>(args)...
    {
    }

    template <std::size_t N>
    constexpr auto& arg(hana::size_t<N> arg = {})
    {
        using T = typename std::remove_cvref_t<decltype(hana::at_c<N>(hana::tuple_t<Args...>))>::type;
        return ebo_get(*static_cast<ebo<T, std::integral_constant<std::size_t, N>>*>(this));
    }

    template <std::size_t N>
    constexpr const auto& arg(hana::size_t<N> arg = {}) const
    {
        using T = typename std::remove_cvref_t<decltype(hana::at_c<N>(hana::tuple_t<Args...>))>::type;
        return ebo_get(*static_cast<const ebo<T, std::integral_constant<std::size_t, N>>*>(this));
    }
};

template <class... Args>
struct Add : VarBase, ExprBase<std::make_index_sequence<sizeof...(Args)>, Args...>
{
    using Base = ExprBase<std::make_index_sequence<sizeof...(Args)>, Args...>;

    template <class... Args2>
    constexpr explicit Add(Args2&&... args)
        : Base(std::forward<Args2>(args)...) {}

    template <class... Param>
    constexpr auto eval(const hana::tuple<Param...>& p) const
    {
        return hana::fold_left(hana::range_c<std::size_t, 0, sizeof...(Args)>, [this, &p](auto l, auto r) {
            return evalImpl(this->arg(l), p) + evalImpl(this->arg(r), p);
        });
    }
};

namespace ADL
{
template <class Left, class Right>
constexpr auto operator+(Left l, Right r)
{
    return Add<Left, Right>{l, r};
}
}  // namespace ADL


template <std::size_t Id>
struct Var : VarBase
{
    template <class... Param>
    constexpr auto eval(const hana::tuple<Param...>& p) const
    {
        return hana::at_c<Id>(p);
    }
};

}  // namespace AutoDiff