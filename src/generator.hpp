#pragma once
#include <coroutine>
#include <exception>
#include <fmt/core.h>
#include <utility>
#include <variant>
template <typename T> class Generator {
public:
  struct promise_type {
    auto get_return_object() noexcept { return Generator{*this}; }
    std::suspend_always initial_suspend() const noexcept { return {}; };
    std::suspend_always final_suspend() const noexcept { return {}; }

    std::suspend_always yield_value(const T &val) noexcept(
        std::is_nothrow_copy_constructible_v<T>) {
      m_value = val;
      return {};
    }

    void return_void() const noexcept {}
    void unhandled_exception() noexcept(
        std::is_nothrow_copy_constructible_v<std::exception_ptr>) {
      m_value = std::current_exception();
    }

    template <typename Expr> Expr &&await_transform(Expr &&expr) {
      static_assert(sizeof(expr) == 0,
                    "co_await is not supported for generators");
      return std::forward<Expr>(expr);
    }

    void rethrow_if_fail() {
      if (m_value.index() == 1) {
        std::rethrow_exception(std::get<1>(m_value));
      }
    }

    std::variant<T, std::exception_ptr> m_value;
  };
  using handle_t = std::coroutine_handle<promise_type>;

  class iterator {

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T const *;
    using reference = T const &;

    iterator(handle_t handle) : m_handle(handle) {}

    bool operator==(iterator other) const noexcept {
      return m_handle == other.m_handle;
    }

    bool operator!=(iterator other) const noexcept {
      return m_handle != other.m_handle;
    }

    iterator &operator++() {
      m_handle.resume();
      if (m_handle.done()) {
        promise_type &promise = m_handle.promise();
        m_handle = nullptr;
        promise.rethrow_if_fail();
      }

      return *this;
    }

    iterator operator++(int) = delete;

    T &operator*() const { return std::get<0>(m_handle.promise().m_value); }

  private:
    handle_t m_handle;
  };

  iterator begin() {
    if (!m_handle) {
      fmt::print("Bye");
      return handle_t(nullptr);
    }

    m_handle.resume();

    if (m_handle.done()) {
      m_handle.promise().rethrow_if_fail();
      m_handle = nullptr;
    }
    return m_handle;
  }

  iterator end() { return handle_t(nullptr); }

  Generator(const Generator &) = delete;
  Generator &operator=(const Generator &) = delete;

  Generator(Generator &&other) noexcept : m_handle(other.m_handle) {
    other.m_handle = nullptr;
  }
  Generator &operator=(Generator &&other) noexcept {
    if (m_handle)
      m_handle.destroy();
    m_handle = std::exchange(other.m_handle, nullptr);
    return *this;
  }
  ~Generator() {
    if (m_handle)
      m_handle.destroy();
  }

private:
  explicit Generator(promise_type &promise) noexcept
      : m_handle(handle_t::from_promise(promise)) {}
  std::coroutine_handle<promise_type> m_handle;
};
