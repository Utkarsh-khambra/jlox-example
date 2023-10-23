#pragma once
#include "token.hpp"
#include <fmt/format.h>
#include <span>
#include <variant>

struct UnaryExpr;
struct BinaryExpr;
struct Literal;

using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
using BinaryExprPtr = std::unique_ptr<BinaryExpr>;
using LiteralPtr = std::unique_ptr<Literal>;

using Expr = std::variant<std::unique_ptr<Literal>, std::unique_ptr<UnaryExpr>,
                          std::unique_ptr<BinaryExpr>>;

struct BinaryExpr {
  Expr lexpr;
  TokenType opr;
  Expr rexpr;
  BinaryExpr(Expr expr, TokenType oper, Expr right_expr)
      : lexpr(std::move(expr)), opr(oper), rexpr(std::move(right_expr)) {}
};

struct UnaryExpr {
  Expr expr;
  TokenType opr;
  UnaryExpr(TokenType oper, Expr expression)
      : expr(std::move(expression)), opr(oper) {}
};

// TODO see if some better repre is possible
using ObjectType = std::variant<std::string, float>;
struct Literal {
  TokenType type;
  std::optional<ObjectType> value;
  Literal(Token t) : type(t.type()), value(t.value()) {}
  Literal(TokenType t) : type(t), value(std::nullopt) {}
  Literal(float v) : type(TokenType::Number), value(v) {}
  Literal(std::string_view v)
      : type(TokenType::String), value(std::string(v)) {}
};

class Parser {
public:
  Expr parse(std::span<Token> tokens);

private:
  constexpr Expr expression() noexcept;
  constexpr Expr equality() noexcept;
  constexpr Expr comparison() noexcept;
  constexpr Expr term() noexcept;
  constexpr Expr factor() noexcept;
  constexpr Expr unary() noexcept;
  constexpr Expr primary() noexcept;

  inline constexpr bool is_current(TokenType type) noexcept {
    return m_current_token.type() == type;
  }

  template <typename... Ts> bool match_any(Ts... Args) noexcept {
    bool b = (is_current(Args) || ...);
    auto s = (fmt::format("{} ", Args) + ...);
    // fmt::print("Tried to match {} with {} got {}\n", m_current_token.type(),
    // s, b);
    return b;
  }

  // Return true token are available
  bool next_token() noexcept {
    bool ret = false;
    if (m_token_stream.size() >= 1) {
      ret = true;
      m_token_stream = m_token_stream.subspan(1);
      m_current_token = m_token_stream.front();
    }
    return ret;
  }
  Token m_current_token;
  std::span<Token> m_token_stream;
};

namespace fmt {
template <> struct formatter<Expr> {

  constexpr auto parse(format_parse_context &ctx)
      -> format_parse_context::iterator {
    // [ctx.begin(), ctx.end()) is a character range that contains a part of
    // the format string starting from the format specifications to be parsed,
    // e.g. in
    //
    //   fmt::format("{:f} - point of interest", point{1, 2});
    //
    // the range will contain "f} - point of interest". The formatter should
    // parse specifiers until '}' or the end of the range. In this example
    // the formatter should parse the 'f' specifier and return an iterator
    // pointing to '}'.

    // Please also note that this character range may be empty, in case of
    // the "{}" format string, so therefore you should check ctx.begin()
    // for equality with ctx.end().

    // Parse the presentation format and store it in the formatter:
    auto it = ctx.begin(), end = ctx.end();
    // Check if reached the end of the range:

    // Return an iterator past the end of the parsed range:
    return it;
  }

  auto format(const Expr &t, format_context &ctx) const
      -> format_context::iterator {
    if (std::holds_alternative<LiteralPtr>(t)) {
      return fmt::format_to(ctx.out(), "{}", std::get<LiteralPtr>(t)->type);
    }
    if (std::holds_alternative<UnaryExprPtr>(t)) {
      const auto &ptr = std::get<UnaryExprPtr>(t);
      return fmt::format_to(ctx.out(), "({} {})", ptr->opr, ptr->expr);
    }
    if (std::holds_alternative<BinaryExprPtr>(t)) {
      const auto &ptr = std::get<BinaryExprPtr>(t);
      return fmt::format_to(ctx.out(), "({} {} {})", ptr->lexpr, ptr->opr,
                            ptr->rexpr);
    }
  }
};
} // namespace fmt
