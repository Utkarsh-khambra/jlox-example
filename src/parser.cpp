#include "parser.hpp"

using enum TokenType;
// TODO make m_token_stream better, I think this design may pose difficulty
// later on
Expr Parser::parse(std::span<Token> tokens) {
  m_token_stream = tokens;
  m_current_token = m_token_stream.front();
  return expression();
}

constexpr Expr Parser::expression() noexcept {
  // fmt::print("expression {}\n", m_current_token.type());

  return equality();
}

constexpr Expr Parser::equality() noexcept {
  // fmt::print("equality {}\n", m_current_token.type());
  auto expr = comparison();
  using enum TokenType;
  while (match_any(BangEqual, Equal)) {
    auto opr = m_current_token;
    next_token();
    auto right_expr = comparison();
    expr = std::make_unique<BinaryExpr>(std::move(expr), opr.type(),
                                        std::move(right_expr));
    // if (!next_token())
    //   break;
  }
  return expr;
}

constexpr Expr Parser::comparison() noexcept {
  // fmt::print("comparison {}\n", m_current_token.type());
  auto expr = term();
  using enum TokenType;
  while (match_any(Greater, GreaterEqual, Less, LessEqual)) {
    auto opr = m_current_token;
    next_token();
    auto right_expr = term();
    expr = std::make_unique<BinaryExpr>(std::move(expr), opr.type(),
                                        std::move(right_expr));
    // if (!next_token())
    //   break;
  }
  return expr;
}

constexpr Expr Parser::term() noexcept {
  // fmt::print("term {}\n", m_current_token.type());
  auto expr = factor();
  while (match_any(Plus, Minus)) {
    auto opr = m_current_token;
    next_token();
    auto right_expr = factor();
    expr = std::make_unique<BinaryExpr>(std::move(expr), opr.type(),
                                        std::move(right_expr));
    // if (!next_token())
    //   break;
  }
  return expr;
}

constexpr Expr Parser::factor() noexcept {
  // fmt::print("factor {}\n", m_current_token.type());
  auto expr = unary();
  while (match_any(Slash, Star)) {
    auto opr = m_current_token;
    next_token();
    auto right_expr = unary();
    expr = std::make_unique<BinaryExpr>(std::move(expr), opr.type(),
                                        std::move(right_expr));
    // if (!next_token())
    //   break;
  }
  return expr;
}

constexpr Expr Parser::unary() noexcept {
  // TODO Fix this
  // if (!next_token())
  //   return std::make_unique<Literal>(0);
  // fmt::print("unary {}\n", m_current_token.type());
  if (match_any(Bang, Minus)) {
    auto opr = m_current_token;
    auto expr = unary();
    return std::make_unique<UnaryExpr>(opr.type(), std::move(expr));
  }
  return primary();
}

constexpr Expr Parser::primary() noexcept {
  // fmt::print("primary {}\n", m_current_token.type());
  if (match_any(Number, String, True, False, Nil, EoF)) {
    auto matched_token = m_current_token;
    next_token();
    return std::make_unique<Literal>(matched_token);
  }
  // TODO check this for when should next_token be called
  if (match_any(LeftParen) && next_token()) {
    auto expr = expression();
    // TODO better error handling
    if (!match_any(RightParen))
      abort();
    next_token();
    return expr;
  }
}
