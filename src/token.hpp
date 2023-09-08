#pragma once
#include <cstddef>
#include <fmt/format.h>
#include <string>
enum class TokenType {
  // Sinle char
  LeftParen,
  RightParen,
  LeftBrace,
  RightBrace,
  Comma,
  Dot,
  Minus,
  Plus,
  Semicolon,
  Slash,
  Star,

  // Two chars
  Bang,
  BangEqual,
  Assignment,
  Equal,
  Greater,
  GreaterEqual,
  Less,
  LessEqual,

  Identifier,
  String,
  Number,

  // Multi char
  And,
  Class,
  Else,
  For,
  While,
  False,
  True,
  If,
  Nil,
  Or,
  Print,
  Return,
  Super,
  This,
  Var
};

enum class LineOffset : size_t {};
enum class ColumnOffset : size_t {};

LineOffset operator++(LineOffset line) noexcept;

class Token {
public:
  Token() {}
  Token(TokenType type, std::string value, LineOffset line_nr,
        ColumnOffset col_nr = ColumnOffset{0})
      : m_line_nr(line_nr), m_col_nr(col_nr), m_type(type), m_value(value) {}
  TokenType type() const noexcept { return m_type; }

private:
  LineOffset m_line_nr;
  ColumnOffset m_col_nr;
  TokenType m_type;
  std::string m_value;
};

namespace fmt {
template <> struct formatter<TokenType> {

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

  auto format(TokenType t, format_context &ctx) const
      -> format_context::iterator {
    using enum TokenType;
    switch (t) {
    case LeftParen: {
      return fmt::format_to(ctx.out(), "LeftParen");
    }
    case RightParen: {
      return fmt::format_to(ctx.out(), "RightParen");
    }
    case LeftBrace: {
      return fmt::format_to(ctx.out(), "LeftBrace");
    }
    case RightBrace: {
      return fmt::format_to(ctx.out(), "RightBrace");
    }
    case Comma: {
      return fmt::format_to(ctx.out(), "Comma");
    }
    case Dot: {
      return fmt::format_to(ctx.out(), "Dot");
    }
    case Minus: {
      return fmt::format_to(ctx.out(), "Minus");
    }
    case Plus: {
      return fmt::format_to(ctx.out(), "Plus");
    }
    case Semicolon: {
      return fmt::format_to(ctx.out(), "Semicolon");
    }
    case Slash: {
      return fmt::format_to(ctx.out(), "Slash");
    }
    case Star: {
      return fmt::format_to(ctx.out(), "Star");
    }
    case Bang: {
      return fmt::format_to(ctx.out(), "Bang");
    }
    case BangEqual: {
      return fmt::format_to(ctx.out(), "BangEqual");
    }
    case Assignment: {
      return fmt::format_to(ctx.out(), "Assignment");
    }
    case Equal: {
      return fmt::format_to(ctx.out(), "Equal");
    }
    case Greater: {
      return fmt::format_to(ctx.out(), "Greater");
    }
    case GreaterEqual: {
      return fmt::format_to(ctx.out(), "GreaterEqual");
    }
    case Less: {
      return fmt::format_to(ctx.out(), "Less");
    }
    case LessEqual: {
      return fmt::format_to(ctx.out(), "LessEqual");
    }
    case Identifier: {
      return fmt::format_to(ctx.out(), "Identifier");
    }
    case String: {
      return fmt::format_to(ctx.out(), "String");
    }
    case Number: {
      return fmt::format_to(ctx.out(), "Number");
    }
    case And: {
      return fmt::format_to(ctx.out(), "And");
    }
    case Class: {
      return fmt::format_to(ctx.out(), "Class");
    }
    case Else: {
      return fmt::format_to(ctx.out(), "Else");
    }
    case For: {
      return fmt::format_to(ctx.out(), "For");
    }
    case While: {
      return fmt::format_to(ctx.out(), "While");
    }
    case False: {
      return fmt::format_to(ctx.out(), "False");
    }
    case True: {
      return fmt::format_to(ctx.out(), "True");
    }
    case If: {
      return fmt::format_to(ctx.out(), "If");
    }
    case Nil: {
      return fmt::format_to(ctx.out(), "Nil");
    }
    case Or: {
      return fmt::format_to(ctx.out(), "Or");
    }
    case Print: {
      return fmt::format_to(ctx.out(), "Print");
    }
    case Return: {
      return fmt::format_to(ctx.out(), "Return");
    }
    case Super: {
      return fmt::format_to(ctx.out(), "Super");
    }
    case This: {
      return fmt::format_to(ctx.out(), "This");
    }
    case Var: {
      return fmt::format_to(ctx.out(), "Var");
    }
    default:
      abort();
    }
  }
};
} // namespace fmt
