#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <scanner.hpp>

TEST_CASE("Tokenization", "[Scanner]") {
  Scanner scanner;
  std::string src =
      R"=(() == != > < for if else var while  <= "Man this is working alright I hope" 34325235235  )=";
  std::vector<TokenType> types{
      TokenType::LeftParen, TokenType::RightParen, TokenType::Equal,
      TokenType::BangEqual, TokenType::Greater,    TokenType::Less,
      TokenType::For,       TokenType::If,         TokenType::Else,
      TokenType::Var,       TokenType::While,      TokenType::LessEqual,
      TokenType::String,    TokenType::Number};
  size_t index = 0;
  for (auto token : scanner.tokenize(src)) {
    REQUIRE(token);
    REQUIRE(token.value().type() == types[index++]);
  }
}

TEST_CASE("Tokenization.hello_world", "[Scanner]") {
  Scanner scanner;
  std::string src = R"=(print "Hello";)=";
  std::vector<TokenType> types{TokenType::Identifier, TokenType::String,
                               TokenType::Semicolon};
  size_t index = 0;
  for (auto token : scanner.tokenize(src)) {
    REQUIRE(token);
    REQUIRE(token.value().type() == types[index++]);
  }
}

TEST_CASE("Tokenization.fibonacci_series", "[Scanner]") {
  Scanner scanner;
  std::string src =
      R"=(def fib(n){
        if (n < 3){
            return 1;
        }
        return fib(n-1) + fib(n-2);
      }
      )=";
  std::vector<TokenType> types{
      TokenType::Def,        TokenType::Identifier, TokenType::LeftParen,
      TokenType::Identifier, TokenType::RightParen, TokenType::LeftBrace,
      TokenType::If,         TokenType::LeftParen,  TokenType::Identifier,
      TokenType::Less,       TokenType::Number,     TokenType::RightParen,
      TokenType::LeftBrace,  TokenType::Return,     TokenType::Number,
      TokenType::Semicolon,  TokenType::RightBrace, TokenType::Return,
      TokenType::Identifier, TokenType::LeftParen,  TokenType::Identifier,
      TokenType::Minus,      TokenType::Number,     TokenType::RightParen,
      TokenType::Plus,       TokenType::Identifier, TokenType::LeftParen,
      TokenType::Identifier, TokenType::Minus,      TokenType::Number,
      TokenType::RightParen, TokenType::Semicolon,  TokenType::RightBrace};
  size_t index = 0;
  for (auto token : scanner.tokenize(src)) {
    REQUIRE(token);
    REQUIRE(token.value().type() == types[index++]);
  }
}
