#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <fmt/core.h>
#include <parser.hpp>
#include <scanner.hpp>

TEST_CASE("ASTGeneration.Expr", "[Parser]") {
  Scanner scanner;
  Parser parser;
  std::string src = R"=(34+28-12/3)=";
  std::vector<Token> tokens;
  for (auto i : scanner.tokenize(src)) {
    tokens.push_back(i.value());
  }
  auto expr = parser.parse(tokens);
  REQUIRE("((Number Plus Number) Minus (Number Slash Number))" ==
          fmt::format("{}", expr));
}
