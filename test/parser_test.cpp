#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <fmt/core.h>
#include <interpreter.hpp>
#include <parser.hpp>
#include <scanner.hpp>

TEST_CASE("ASTGeneration.Expr", "[Parser]") {
  Scanner scanner;
  Parser parser;
  SECTION("Simple Expr") {
    std::string src = R"=(34+28-12/3)=";
    std::vector<Token> tokens;
    for (auto i : scanner.tokenize(src)) {
      tokens.push_back(i.value());
    }
    auto expr = parser.parse(tokens);
    REQUIRE("((Number Plus Number) Minus (Number Slash Number))" ==
            fmt::format("{}", expr));
  }

  SECTION("Parenthesized expr") {
    std::string src = R"=(34+(28-12)/3)=";
    std::vector<Token> tokens;
    for (auto i : scanner.tokenize(src)) {
      tokens.push_back(i.value());
    }
    auto expr = parser.parse(tokens);
    REQUIRE("(Number Plus ((Number Minus Number) Slash Number))" ==
            fmt::format("{}", expr));
  }
}

TEST_CASE("ASTEvaluation.Expr", "[Parser]") {
  Scanner scanner;
  Parser parser;
  SECTION("Simple Expr") {
    std::string src = R"=(34+28-12/3)=";
    std::vector<Token> tokens;
    for (auto i : scanner.tokenize(src)) {
      tokens.push_back(i.value());
    }
    auto expr = parser.parse(tokens);
    auto result = std::visit(Interpreter(), expr);
    REQUIRE(58 == std::get<float>(result.value.value()));
  }

  SECTION("Parenthesized expr") {
    std::string src = R"=(34+(28-12)/3)=";
    std::vector<Token> tokens;
    for (auto i : scanner.tokenize(src)) {
      tokens.push_back(i.value());
    }
    auto expr = parser.parse(tokens);
    auto result = std::visit(Interpreter(), expr);
    REQUIRE(std::get<float>(result.value.value()) == ((16.f / 3.f) + 34.f));
  }
}
