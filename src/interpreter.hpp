#pragma once
#include "parser.hpp"

using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
using BinaryExprPtr = std::unique_ptr<BinaryExpr>;
using LiteralPtr = std::unique_ptr<Literal>;

class Interpreter {
public:
  auto operator()(const Literal &l) const noexcept { return l.value; }
  auto operator()(const UnaryExprPtr &expr) const noexcept {
    return evaluate_unary(*expr);
  }
  auto operator()(const BinaryExprPtr &expr) const noexcept {
    return evaluate_binary(*expr);
  }

private:
  Literal evaluate(const Expr &expr) const noexcept {
    if (std::holds_alternative<std::unique_ptr<UnaryExpr>>(expr))
      evaluate_unary(*std::get<std::unique_ptr<UnaryExpr>>(expr));
    else if (std::holds_alternative<std::unique_ptr<BinaryExpr>>(expr))
      evaluate_binary(*std::get<std::unique_ptr<BinaryExpr>>(expr));
  }

  Literal evaluate_unary(const UnaryExpr &expr) const noexcept {
    auto value = evaluate(expr.expr);
    return perform_unary_op(value, expr.opr);
  }
  Literal evaluate_binary(const BinaryExpr &expr) const noexcept {
    auto value_left = evaluate(expr.lexpr);
    auto value_right = evaluate(expr.rexpr);
    return perform_binary_op(value_left, expr.opr, value_right);
  }

  Literal perform_unary_op(Literal operand, TokenType opr) const noexcept;
  Literal perform_binary_op(Literal left_operand, TokenType opr,
                            Literal right_operand) const noexcept;
};
