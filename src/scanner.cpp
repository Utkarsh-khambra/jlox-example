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
#include <unordered_map>

class SourceCode {
public:
  using Result = std::expected<Token, int>;
  constexpr SourceCode(std::string_view src) : m_source_code(src) {}

  // Matches the nth character to be a in src;
  constexpr bool match_next(char a, size_t n) {
    assert(m_source_code.size() >= n && n != 0);
    if (a == m_source_code[n - 1]) {
      consume(n);
      return true;
    }
    return false;
  }
  // Consume n chars from src
  constexpr std::string_view consume(size_t n) noexcept {

    if (n <= m_source_code.size()) {
      auto str = m_source_code.substr(0, n);
      m_source_code.remove_prefix(n);
      return str;
    }
    return {};
  }

  template <typename F>
  constexpr std::string_view consume_till(F &&pred) noexcept {
    auto itr = std::ranges::find_if(m_source_code, pred);
    return consume_upto(itr);
  }

  // Caller must have consumed starting quoting
  constexpr Result get_string() noexcept {
    auto itr = std::ranges::find(m_source_code, '"');
    auto str = consume_upto(itr);
    // To consume remaining quote
    consume(1);
    if (itr == m_source_code.end())
      return std::unexpected(1);
    return Token(TokenType::String, std::string(str), LineOffset{0});
  }

  constexpr Token get_number() noexcept {
    auto itr = std::ranges::find_if(m_source_code, [](char a) noexcept {
      return !std::isdigit(a) && a != '.';
    });
    return Token(TokenType::Number, std::string(consume_upto(itr)),
                 LineOffset{0});
  }

  constexpr Token get_identifier(char starting_char) noexcept {
    // Only alphas are allowed in identifiers;
    auto indentifier =
        starting_char + std::string(consume_till(
                            [](char a) noexcept { return !std::isalpha(a); }));
    return match_ident_or_keyword(std::move(indentifier));
  }

  constexpr char peek() const noexcept { return m_source_code.front(); }

  explicit constexpr operator bool() const noexcept {
    return !m_source_code.empty();
  }
  size_t s() const noexcept { return m_source_code.size(); }

private:
  Token match_ident_or_keyword(std::string str) noexcept {
    auto itr = keyword_map.find(str);
    if (itr != keyword_map.end())
      return Token(itr->second, std::move(str), LineOffset{0});
    return Token(TokenType::Identifier, std::move(str), LineOffset{0});
  }

  // Doesn't include the itr in resulting itr;
  template <std::forward_iterator Itr>
  std::string_view consume_upto(Itr itr) noexcept {
    auto str = m_source_code.substr(
        0, static_cast<size_t>(std::distance(m_source_code.begin(), itr)));
    m_source_code.remove_prefix(
        static_cast<size_t>(std::distance(m_source_code.begin(), itr)));
    return str;
  }
  const std::unordered_map<std::string, TokenType> keyword_map{
      {"for", TokenType::For},       {"while", TokenType::While},
      {"if", TokenType::If},         {"else", TokenType::Else},
      {"class", TokenType::Class},   {"false", TokenType::False},
      {"true", TokenType::True},     {"nil", TokenType::Nil},
      {"or", TokenType::Or},         {"and", TokenType::And},
      {"return", TokenType::Return}, {"super", TokenType::Super},
      {"this", TokenType::This},     {"var", TokenType::Var},
      {"def", TokenType::Def}};
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
    char const c = source_code.consume(1).front();
    switch (c) {
    case '(': {
      co_yield Token(LeftParen, "(", line_nr);
      break;
    }
    case ')': {
      co_yield Token(RightParen, ")", line_nr);
      break;
    }
    case '{': {
      co_yield Token(LeftBrace, "{", line_nr);
      break;
    }
    case '}': {
      co_yield Token(RightBrace, "}", line_nr);
      break;
    }
    case ',': {
      co_yield Token(Comma, ",", line_nr);
      break;
    }
    case '.': {
      co_yield Token(Dot, ".", line_nr);
      break;
    }
    case '-': {
      co_yield Token(Minus, "-", line_nr);
      break;
    }
    case '+': {
      co_yield Token(Plus, "+", line_nr);
      break;
    }
    case ';': {
      co_yield Token(Semicolon, ";", line_nr);
      break;
    }
    case '*': {
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
        co_yield Token(TokenType::Slash, "/", line_nr);
      }
      break;
    }
    case '"': {
      co_yield source_code.get_string();
      break;
    }
    case ' ':
      [[fallthrough]];
    case '\t':
      [[fallthrough]];
    case '\r':
      [[fallthrough]];
    case '\n': {
      ++line_nr;
      break;
    }
    default: {
      if (std::isdigit(c)) {
        co_yield source_code.get_number();
        break;
      } else if (std::isalpha(c)) {
        co_yield source_code.get_identifier(c);
        break;
      }
      co_yield std::unexpected(4);
    }
    }
  }
}

// void Scanner::run_prompt() {
//   std::string current_line;
//   while (std::getline(std::cin, current_line)) {
//     eval_statement(current_line);
//   }
// }

// void Scanner::eval_statement(std::string_view statement) {
//
//   fmt::print("{}\n", statement);
// }
