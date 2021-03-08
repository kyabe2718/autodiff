#pragma once
#include <type_traits>
#include <utility>

namespace vessel
{
namespace detail
{
template <std::size_t Id>
struct ArgIdx;

template <class ArgIdx, class T>
struct tuple_elm;

template <class T>
concept is_empty_elm = std::is_empty_v<T>&& std::is_default_constructible_v<T>;

template <std::size_t Idx, class T>
requires is_empty_elm<T> struct tuple_elm<ArgIdx<Idx>, T>
{
    constexpr tuple_elm() = default;

    template <class T2>
    constexpr explicit tuple_elm(T2&& v)
    {
        T(std::forward<T2>(v));
    }
};

template <std::size_t Idx, class T>
struct tuple_elm<ArgIdx<Idx>, T>
{
    constexpr tuple_elm() = default;

    template <class T2>
    constexpr explicit tuple_elm(T2&& arg)
        : data(std::forward<T2>(arg)) {}

    [[no_unique_address]] T data;
};

template <std::size_t Idx, class T>
constexpr T get_tuple_elm(const tuple_elm<ArgIdx<Idx>, T>& elm) requires is_empty_elm<T>
{
    return T{};
}
template <std::size_t Idx, class T>
constexpr T& get_tuple_elm(tuple_elm<ArgIdx<Idx>, T>& elm)
{
    return elm.data;
}
template <std::size_t Idx, class T>
constexpr const T& get_tuple_elm(const tuple_elm<ArgIdx<Idx>, T>& elm)
{
    return elm.data;
}
template <std::size_t Idx, class T>
constexpr T&& get_tuple_elm(tuple_elm<ArgIdx<Idx>, T>&& elm)
{
    return std::move(elm.data);
}

template <class...>
struct tuple_impl;

template <std::size_t... I, class... Args>
struct tuple_impl<std::index_sequence<I...>, Args...>
    : public tuple_elm<ArgIdx<I>, Args>...
{
    template <class... Args2>
    constexpr tuple_impl(Args2&&... args) requires(sizeof...(Args) == sizeof...(Args2))
        : tuple_elm<ArgIdx<I>, Args>(std::forward<Args2>(args))... {}

    constexpr tuple_impl() = default;
};
}  // namespace detail

template <class... Args>
struct tuple : detail::tuple_impl<std::make_index_sequence<sizeof...(Args)>, Args...>
{
private:
    using Impl = detail::tuple_impl<std::make_index_sequence<sizeof...(Args)>, Args...>;

public:
    using Impl::Impl;
    static constexpr std::size_t size = sizeof...(Args);

    template <std::size_t Idx>
    constexpr decltype(auto) get(boost::hana::size_t<Idx> = {})
    {
        return detail::get_tuple_elm<Idx>(*this);
    }

    template <std::size_t Idx>
    constexpr decltype(auto) get(boost::hana::size_t<Idx> = {}) const
    {
        return detail::get_tuple_elm<Idx>(*this);
    }
};

template <class... Args>
constexpr auto make_tuple(Args&&... args)
{
    return tuple<std::unwrap_ref_decay_t<Args>...>{std::forward<Args>(args)...};
}
}  // namespace vessel
