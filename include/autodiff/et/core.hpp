#pragma once

#include <tuple>
#include <concepts>
#include <functional>
#include <boost/hana.hpp>

#include <vessel/tuple.hpp>

namespace AutoDiff::ET
{
namespace hana = boost::hana;

namespace ADL
{
struct ADLTag;

template <class T>
auto hasADLTagImpl(T*) -> std::true_type;
}  // namespace ADL

namespace detail
{
auto hasADLTagImpl(...) -> std::false_type;

template <class T>
struct HasADLTag : decltype(hasADLTagImpl(std::declval<T*>()))
{};
}  // namespace detail

template <class, class = ADL::ADLTag>
struct ETFunc;

template <class F, class... Args>
requires std::is_empty_v<F> struct ETFunc<F(Args...), ADL::ADLTag>
{
    template <class... Args2>
    constexpr explicit ETFunc(Args2&&... args) requires(sizeof...(Args) == sizeof...(Args2))
        : args(vessel::make_tuple(std::forward<Args2>(args)...)) {}

    constexpr explicit ETFunc() requires(std::is_empty_v<Args>&&...) = default;

    template <class... Param>
    constexpr decltype(auto) apply(const hana::tuple<Param...>& p) const
    {
        return hana::unpack(hana::make_range(hana::size_c<0>, hana::size_c<sizeof...(Args)>),
            hana::on(F{}, [this, &p](auto idx) { return eval(this->args.get(idx), p); }));
        // return hana::unpack(args, hana::on(F{}, [&p](auto&& arg) { return eval(std::forward<decltype(arg)>(arg), p); }));
    }

private:
    template <class T, class... Params>
    constexpr static decltype(auto) eval(T&& v, const hana::tuple<Params...>& p)
    {
        if constexpr (detail::HasADLTag<std::remove_cvref_t<T>>::value) {
            return std::forward<T>(v).apply(p);
        } else {
            return v;
        }
    }

    [[no_unique_address]] vessel::tuple<Args...> args;
};

template <std::size_t Id, class = ADL::ADLTag>
struct ETVar
{
    template <class... Param>
    constexpr static auto apply(const hana::tuple<Param...>& p)
    {
        return hana::at_c<Id>(p);
    }
};

namespace detail
{
template <std::size_t... I, class F>
constexpr auto genETFuncImpl(std::index_sequence<I...>, F&& func)
{
    return std::forward<F>(func)(ETVar<I>{}...);
}
}  // namespace detail

template <std::size_t ArgNum, class F>
constexpr auto genETFunc(F&& func)
{
    return detail::genETFuncImpl(std::make_index_sequence<ArgNum>(), std::forward<F>(func));
}
}  // namespace AutoDiff::ET
