#pragma once
#include <cstddef>
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
  BandEqual,
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

class Token {
public:
  Token(TokenType type, std::string value, LineOffset line_nr,
        ColumnOffset col_nr)
      : m_line_nr(line_nr), m_col_nr(col_nr), m_type(type), m_value(value) {}

private:
  LineOffset m_line_nr;
  ColumnOffset m_col_nr;
  TokenType m_type;
  std::string m_value;
};
