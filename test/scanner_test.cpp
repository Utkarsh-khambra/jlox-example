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
    if (!token)
      fmt::print("{}\n", token.error());
    else
      fmt::print("{} {}\n", token.value().type(), types[index]);
    REQUIRE(token);
    REQUIRE(token.value().type() == types[index++]);
  }
}
