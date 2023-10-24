#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <fmt/core.h>
#include <interpreter.hpp>
#include <parser.hpp>
#include <scanner.hpp>

auto create_scenerio(std::string_view src) {
  Scanner scanner;
  Parser parser;
  std::vector<Token> tokens;
  for (auto i : scanner.tokenize(src)) {
    tokens.push_back(i.value());
  }
  return parser.parse(tokens);
}

TEST_CASE("ExpressionAST", "[Parser]") {

  SECTION("Comparison") {
    SECTION("Greate") {

      std::string src = R"=(34 >2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number Greater Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }
    SECTION("GreateEqual") {

      std::string src = R"=(34 >=2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number GreaterEqual Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }
    SECTION("Less") {

      std::string src = R"=(34 <2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number Less Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE_FALSE(result);
    }
    SECTION("LessEqual") {

      std::string src = R"=(34 <=2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number LessEqual Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE_FALSE(result);
    }
    SECTION("Equal") {
      std::string src = R"=(34==2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number Equal Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE_FALSE(result);
    }
    SECTION("Equal") {
      std::string src = R"=(34==34)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number Equal Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }
    SECTION("NotEqual") {
      std::string src = R"=(34!=2)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number BangEqual Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }
    SECTION("NotEqual") {
      std::string src = R"=(34!=34)=";
      auto expr = create_scenerio(src);
      REQUIRE("(Number BangEqual Number)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE_FALSE(result);
    }
    SECTION("String") {

      std::string src = R"=("This is test"== "This is test")=";
      auto expr = create_scenerio(src);
      REQUIRE("(String Equal String)" == fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }

    SECTION("Combined") {

      std::string src = R"=(54>2!=5)=";
      auto expr = create_scenerio(src);
      REQUIRE("((Number Greater Number) BangEqual Number)" ==
              fmt::format("{}", expr));
      auto result = std::get<bool>(std::visit(Interpreter(), expr));
      REQUIRE(result);
    }
  }

  SECTION("Simple Expr") {
    std::string src = R"=(34+28-12/3)=";
    auto expr = create_scenerio(src);
    REQUIRE("((Number Plus Number) Minus (Number Slash Number))" ==
            fmt::format("{}", expr));
    auto result = std::get<float>(std::visit(Interpreter(), expr));
    REQUIRE(result == 58);
  }

  SECTION("Parenthesized expr") {
    std::string src = R"=(34+(28-12)/3)=";
    auto expr = create_scenerio(src);
    REQUIRE("(Number Plus ((Number Minus Number) Slash Number))" ==
            fmt::format("{}", expr));
    auto result = std::get<float>(std::visit(Interpreter(), expr));
    REQUIRE(result == ((16.f / 3.f) + 34.f));
  }
}
