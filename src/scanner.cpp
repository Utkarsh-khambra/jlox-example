#include "scanner.hpp"
#include "generator.hpp"
#include <cassert>
#include <concepts>
#include <coroutine>
#include <cstdlib>
#include <expected>
#include <fmt/core.h>
#include <fstab.h>
#include <fstream>
#include <iostream>

class SourceCode {
public:
  SourceCode(std::string_view src) : m_source_code(src) {}

  // Matches the nth character to be a in src;
  constexpr bool match_next(char a, size_t n) {
    assert(m_source_code.size() >= n + 1);
    if (a == m_source_code[n]) {
      consume(n + 1);
      return true;
    }
    consume(1);
    return false;
  }
  // Consume n chars from src
  constexpr void consume(size_t n) noexcept {

    assert(n <= m_source_code.size());
    m_source_code.remove_prefix(n);
  }

  template <typename F> constexpr void consume_till(F &&pred) noexcept {
    auto itr = std::ranges::find_if(m_source_code, pred);
    auto const len = std::distance(m_source_code.begin(), itr);
    consume(len);
  }

  char peek() const noexcept { return m_source_code.front(); }

  explicit operator bool() const noexcept { return !m_source_code.empty(); }
  size_t s() const noexcept { return m_source_code.size(); }

private:
  std::string_view m_source_code;
};

void Scanner::scan(std::string_view filepath) {
  std::ifstream source_file((std::string(filepath)));
  if (!source_file)
    abort();
  std::string source((std::istream_iterator<char>(source_file)),
                     std::istream_iterator<char>());
}

Generator<std::expected<Token, int>> Scanner::tokenize(std::string_view src) {
    SourceCode source_code(src);
  LineOffset line_nr{1};
  while (source_code) {
    using enum TokenType;
    switch (source_code.peek()) {
    case '(': {
      source_code.consume(1);
      co_yield Token(LeftParen, "(", line_nr);
      break;
    }
    case ')': {
      source_code.consume(1);
      co_yield Token(RightParen, ")", line_nr);
      break;
    }
    case '{': {
      source_code.consume(1);
      co_yield Token(LeftBrace, "{", line_nr);
      break;
    }
    case '}': {
      source_code.consume(1);
      co_yield Token(RightBrace, "}", line_nr);
      break;
    }
    case ',': {
      source_code.consume(1);
      co_yield Token(Comma, ",", line_nr);
      break;
    }
    case '.': {
      source_code.consume(1);
      co_yield Token(Dot, ".", line_nr);
      break;
    }
    case '-': {
      source_code.consume(1);
      co_yield Token(Minus, "-", line_nr);
      break;
    }
    case '+': {
      source_code.consume(1);
      co_yield Token(Plus, "+", line_nr);
      break;
    }
    case ';': {
      source_code.consume(1);
      co_yield Token(Semicolon, ";", line_nr);
      break;
    }
    case '*': {
      source_code.consume(1);
      co_yield Token(Star, "*", line_nr);
      break;
    }
    case '!': {
      auto const result = source_code.match_next('=', 1);
      auto const token_type = result ? TokenType::BangEqual : TokenType::Bang;
      auto str = result ? "!=" : "!";
      co_yield Token(token_type, str, line_nr);
      break;
    }
    case '=': {
      auto const result = source_code.match_next('=', 1);
      auto const token_type = result ? TokenType::Equal : TokenType::Assignment;
      auto str = result ? "==" : "=";
      co_yield Token(token_type, str, line_nr);
      break;
    }
    case '<': {
      auto const result = source_code.match_next('=', 1);
      auto const token_type = result ? TokenType::LessEqual : TokenType::Less;
      auto str = result ? "<=" : "<";
      co_yield Token(token_type, str, line_nr);
      break;
    }
    case '>': {
      auto const result = source_code.match_next('=', 1);
      auto const token_type =
          result ? TokenType::GreaterEqual : TokenType::Greater;
      auto str = result ? ">=" : ">";
      co_yield Token(token_type, str, line_nr);
      break;
    }
    case '/': {
      auto const result = source_code.match_next('/', 1);
      if (result) {
        source_code.consume_till([](char a) noexcept { return a == '\n'; });
      } else {
        source_code.consume(1);
        co_yield Token(TokenType::Slash, "/", line_nr);
      }
      break;
    }
    case ' ':
      [[fallthrough]];
    case '\t':
      [[fallthrough]];
    case '\r':
      source_code.consume(1);
      break;
    case '\n':
      ++line_nr;
      source_code.consume(1);
      break;
    default:
      abort();
    }
  }
}

void Scanner::run_prompt() {
  std::string current_line;
  while (std::getline(std::cin, current_line)) {
    eval_statement(current_line);
  }
}

void Scanner::eval_statement(std::string_view statement) {

  fmt::print("{}\n", statement);
}
