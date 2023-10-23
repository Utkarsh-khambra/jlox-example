#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>
Literal Interpreter::perform_unary_op(Literal operand,
                                      TokenType opr) const noexcept {
  // TODO better error handling;
  using enum TokenType;
  switch (opr) {
  case Bang: {
    if (operand.type == True)
      return Literal{False};
    if (operand.type == False)
      return Literal{True};
    abort();
  }
  case Minus: {
    if (operand.type == Number)
      return Literal{-1 * operand.value.value()};
  }
    abort();
  }
  abort();
}
Literal Interpreter::perform_binary_op(Literal left_operand, TokenType opr,
                                       Literal right_operand) const noexcept {
  using enum TokenType;
  switch (opr) {
  case Plus:
    assert(left_operand.value.has_value() && right_operand.value.has_value());
    return *left_operand.value + *right_operand.value;
  case Minus:
    assert(left_operand.value.has_value() && right_operand.value.has_value());
    return *left_operand.value - *right_operand.value;
  case Star:
    assert(left_operand.value.has_value() && right_operand.value.has_value());
    return *left_operand.value * *right_operand.value;
  case Slash:
    assert(left_operand.value.has_value() && right_operand.value.has_value());
    return *left_operand.value / *right_operand.value;
    // case BangEqual:
    //   // assert(left_operand.value.has_value() &&
    //   // right_operand.value.has_value());
    //   assert(left_operand.type == right_operand.type);
    //   assert(left_operand.type != Number ||
    //          (left_operand.value.has_value() &&
    //          right_operand.value.has_value()));
    //   return;
    // case Equal:
    //   assert(left_operand.value.has_value() &&
    //   right_operand.value.has_value()); return *left_operand.value /
    //   *right_operand.value;
    // case Greater:
    // case GreaterEqual:
    // case Less:
    // case LessEqual:
  }
}
