#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <scanner.hpp>

TEST_CASE("Tokenization", "[Scanner]") {
  Scanner scanner;
  std::string src = R"=(() == != > < <=   )=";
  std::vector<TokenType> types{TokenType::LeftParen, TokenType::RightParen,TokenType::Equal,
                               TokenType::BangEqual, TokenType::Greater,
                               TokenType::Less,      TokenType::LessEqual};
  size_t index = 0;
  for (auto token : scanner.tokenize(src)) {
      REQUIRE(token);
      REQUIRE(token.value().type() == types[index++]);
  }
}
