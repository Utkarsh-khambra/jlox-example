#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>

using enum TokenType;
Literal Interpreter::perform_unary_op(Literal operand,
                                      TokenType opr) const noexcept {
  // TODO better error handling;
  switch (opr) {
  case Bang: {
    if (std::holds_alternative<bool>(operand))
      return Literal(!std::get<bool>(operand));
    // TODO better error handling
    abort();
  }
  case Minus: {
    if (std::holds_alternative<float>(operand))
      return Literal(-1 * std::get<float>(operand));
  }
    // TODO better error handling
    abort();
  }
  // TODO better error handling
  abort();
}

template <typename... Args> inline bool is_workable_types(const Literal &obj) {
  return (std::holds_alternative<Args>(obj) || ...);
}

Literal Interpreter::perform_binary_op(Literal left_operand, TokenType opr,
                                       Literal right_operand) const noexcept {
  switch (opr) {
    // TODO better error handling
  case Plus:
    // assert(left_operand.value.has_value() &&
    // right_operand.value.has_value());
    // fmt::print("{} {}", std::get<float>(left_operand),
    //            std::get<float>(right_operand));
    return std::get<float>(left_operand) + std::get<float>(right_operand);
  case Minus:
    // assert(left_operand.value.has_value() &&
    // right_operand.value.has_value());
    // fmt::print("{} {}", std::get<float>(left_operand),
    //            std::get<float>(right_operand));
    return std::get<float>(left_operand) - std::get<float>(right_operand);
  case Star:
    // assert(left_operand.value.has_value() &&
    // right_operand.value.has_value());
    // fmt::print("{} {}", std::get<float>(left_operand),
    //            std::get<float>(right_operand));
    return std::get<float>(left_operand) * std::get<float>(right_operand);
  case Slash:
    // assert(left_operand.value.has_value() &&
    // right_operand.value.has_value());
    // fmt::print("{} {}", std::get<float>(left_operand),
    //            std::get<float>(right_operand));
    return std::get<float>(left_operand) / std::get<float>(right_operand);
  case BangEqual: {
    return left_operand != right_operand;
  }
  case Equal: {
    return left_operand == right_operand;
  }
  case Greater: {
    {
      assert((is_workable_types<float, std::string>(left_operand)) &&
             (is_workable_types<float, std::string>(right_operand)));
      return left_operand > right_operand;
    }
  }
  case GreaterEqual: {
    assert((is_workable_types<float, std::string>(left_operand)) &&
           (is_workable_types<float, std::string>(right_operand)));
    return left_operand >= right_operand;
  }
  case Less: {
    assert((is_workable_types<float, std::string>(left_operand)) &&
           (is_workable_types<float, std::string>(right_operand)));
    return left_operand < right_operand;
  }
  case LessEqual: {
    assert((is_workable_types<float, std::string>(left_operand)) &&
           (is_workable_types<float, std::string>(right_operand)));
    return left_operand <= right_operand;
  }
  }
}
