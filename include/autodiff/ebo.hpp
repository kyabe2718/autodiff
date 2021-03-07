#pragma once

namespace AutoDiff
{
// boost::hanaのebo (empty base optimization) とほぼ同じ

template <class T, class K = void, bool = std::is_empty_v<T> && !std::is_final_v<T>>
struct ebo;

template <class T, class K>
struct ebo<T, K, true> : T
{
    template <class T2>
    constexpr explicit ebo(T2&& data)
        : T(std::forward<T2>(data)) {}
};

template <class T, class K>
struct ebo<T, K, false>
{
    constexpr explicit ebo() = default;

    template <class T2>
    constexpr explicit ebo(T2&& data)
        : data(std::forward<T2>(data)) {}

    T data;
};

template <class T, class K>
constexpr T& ebo_get(ebo<T, K, true>& obj)
{
    return obj;
}
template <class T, class K>
constexpr const T& ebo_get(const ebo<T, K, true>& obj)
{
    return obj;
}
template <class T, class K>
constexpr T&& ebo_get(ebo<T, K, true>&& obj)
{
    return std::move(obj);
}
template <class T, class K>
constexpr T& ebo_get(ebo<T, K, false>& obj)
{
    return obj.data;
}
template <class T, class K>
constexpr const T& ebo_get(const ebo<T, K, false>& obj)
{
    return obj.data;
}
template <class T, class K>
constexpr T&& ebo_get(ebo<T, K, false>&& obj)
{
    return std::move(obj.data);
}
}  // namespace AutoDiff