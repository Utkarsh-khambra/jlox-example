#pragma once
#include <concepts>
#include <initializer_list>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

template <typename T>
concept Unexpected = !
std::is_same_v<T, void> && !std::is_array_v<T>;

template <Unexpected E> class unexpected {
public:
  unexpected() = delete;
  constexpr unexpected(const unexpected &e) = default;
  constexpr unexpected(const unexpected &&e) = default;

  template <typename Err = E> constexpr explicit unexpected(Err &&e);
  template <typename... Args>
    requires(std::is_constructible_v<Args...>)
  constexpr explicit unexpected(std::in_place_t, Args... args) noexcept(
      std::is_nothrow_constructible_v<E, Args...>)
      : m_val(std::forward<Args>(args)...) {}
  template <typename U, typename... Args>
    requires(std::is_constructible_v<E, std::initializer_list<U> &, Args...>)
  constexpr explicit unexpected(
      std::in_place_t, std::initializer_list<U> il,
      Args... args) noexcept(std::
                                 is_nothrow_constructible_v<
                                     E, std::initializer_list<U> &, Args...>)
      : m_val(il, std::forward<Args>(args)...) {}
  constexpr unexpected &operator=(const unexpected &e) = default;
  constexpr unexpected &operator=(const unexpected &&e) = default;

  [[nodiscard]] constexpr const E &error() const &noexcept { return m_val; }
  [[nodiscard]] constexpr E &error() &noexcept { return m_val; }
  [[nodiscard]] constexpr const E &&error() const &&noexcept {
    return std::move(m_val);
  }
  [[nodiscard]] constexpr E &&error() &&noexcept { return std::move(m_val); }

  constexpr void swap(unexpected &e) noexcept(std::is_nothrow_swappable_v<E>)
    requires(std::is_swappable_v<E>)
  {
    using std::swap;
    swap(e.m_val, m_val);
  };

  template <typename Err>
  constexpr friend bool operator==(const unexpected &x,
                                   const unexpected<Err> &y) {
    return x.m_val == y.m_val;
  }
  template <typename Err>
  constexpr friend void swap(const unexpected &x,
                             const unexpected<Err> &y) noexcept {
    x.swap(y);
  }

private:
  E m_val;
};

struct unexpected_t {};

template <typename T>
constexpr bool operator==(const unexpected<T> &x,
                          const unexpected<T> &y) noexcept {
  return x.error() == y.error();
}

template <typename T, typename E> class Expected {
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_function_v<T>);
  static_assert(!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>);
  static_assert(!std::is_same_v<std::remove_cv_t<T>, unexpected_t>);
  static_assert(!std::is_same_v<std::remove_cv_t<T>, unexpected_t>);
  static_assert(!std::is_same_v<std::remove_cv_t<T>, unexpected_t>);

  template <typename U, typename G, typename Unexp = Unexpected<G>>
  static constexpr bool construct_expect =
      std::is_constructible_v<T, Expected<U, G> &> ||
      std::is_constructible_v<T, Expected<U, G>> ||
      std::is_constructible_v<T, const Expected<U, G> &> ||
      std::is_constructible_v<T, const Expected<U, G>> ||
      std::is_convertible_v<Expected<U, G> &, T> ||
      std::is_convertible_v<Expected<U, G>, T> ||
      std::is_convertible_v<const Expected<U, G> &, T> ||
      std::is_convertible_v<const Expected<U, G>, T>;
  std::is_constructible_v<Unexp, Expected<U, G> &> ||
      std::is_constructible_v<Unexp, Expected<U, G>> ||
      std::is_constructible_v<Unexp, const Expected<U, G> &> ||
      std::is_constructible_v<Unexp, const Expected<U, G>>;

  template <typename U, typename G>
  static constexpr bool explicit_conv =
      !std::is_convertible_v<T, U> || !std::is_convertible_v<E, G>;

public:
  constexpr Expected() noexcept(std::is_nothrow_default_constructible_v<T>)
    requires(std::is_nothrow_default_constructible_v<T>)
      : m_val(), m_has_val(true) {}

  Expected(const Expected &e) = default;
  constexpr Expected(const Expected &e) noexcept(
      std::is_nothrow_copy_constructible_v<T>
          &&std::is_nothrow_copy_constructible_v<E>)
    requires(std::is_copy_constructible<T> && std::is_copy_constructible<E> &&
             (!std::is_trivially_copy_constructible_v<T> ||
              !std::is_trivially_copy_constructible_v<E>))
      : m_has_val(e.m_has_val) {
    if (m_has_val)
      std::construct_at(std::addressof(m_val), e.m_val);
    else
      std::construct_at(std::addressof(m_unex), e.m_unex);
  }

  Expected(Expected &&) = default;

  constexpr Expected(Expected &&e) noexcept(
      std::is_nothrow_move_constructible_v<T>
          &&std::is_nothrow_move_constructible_v<E>)
    requires(std::is_move_constructible_v<T> &&
             std::is_move_constructible_v<E> &&
             (!std::is_trivially_move_constructible_v<T> ||
              !std::is_trivially_move_constructible_v<E>))
      : m_has_val(e.m_has_val) {
    if (m_has_val)
      std::construct_at(std::addressof(m_val), std::move(e).m_val);
    else
      std::construct_at(std::addressof(m_unex), std::move(e).m_unex);
  }

  template <typename U, typename G>
    requires(std::is_constructible_v<T, const U &> &&
             std::is_constructible_v<E, const G &> && (!construct_expect<U, G>))
  constexpr explicit(explicit_conv<const U &, const G &>)
      Expected(const Expected<U, G> &e) noexcept(
          std::is_nothrow_constructible_v<T, const U &>
              &&std::is_nothrow_constructible_v<E, const G &>)
      : m_has_val(e.m_has_val) {
    if (m_has_val)
      std::construct_at(std::addressof(m_val), e.m_val);
    else
      std::construct_at(std::addressof(m_val), e.m_unex);
  }

  template <typename U, typename G>
    requires(std::is_constructible_v<T, U> && std::is_constructible_v<E, G> &&
             (!construct_expect<U, G>))
  constexpr explicit(explicit_conv<U, G>) Expected(Expected<U, G> &&e) noexcept(
      std::is_nothrow_constructible_v<T, U>
          &&std::is_nothrow_constructible_v<E, G>)
      : m_has_val(e.m_has_val) {
    if (m_has_val)
      std::construct_at(std::addressof(m_val), std::move(e).m_val);
    else
      std::construct_at(std::addressof(m_val), std::move(e).m_unex);
  }

  template <typename U = T>
    requires(!std::is_same_v<std::remove_cv_t<U>, Expected> &&
             !std::is_same_v<std::remove_cv_t<U>, std::in_place_t> &&
             std::is_constructible_v<T, U>)
  constexpr explicit(!std::is_convertible_v<T, U>)
      Expected(U &&val) noexcept(std::is_nothrow_constructible_v<T, U>)
      : m_val(std::forward<U>(val)) : m_has_val(true) {}

  template <typename G = E>
    requires(std::is_constructible_v<G, E>)
  constexpr explicit(!std::is_convertible_v<G, E>) Expected(
      Unexpected<G> &&unexp) noexcept(std::is_nothrow_constructible_v<G, E>)
      : m_unex(unexp.error()) : m_has_val(false) {}

  template <typename G = E>
    requires(std::is_constructible_v<const G &, E>)
  constexpr explicit(!std::is_convertible_v<const G &, E>)
      Expected(Unexpected<G> &&unexp) noexcept(
          std::is_nothrow_constructible_v<const G &, E>)
      : m_unex(unexp.error()) : m_has_val(false) {}

private:
  union {
    T m_val;
    E m_unex;
  };
  bool m_has_val;
};
