#pragma once
#include "token.hpp"
#include <generator.hpp>
#include <expected>
#include <string_view>
#include <vector>
class Scanner {
public:
  void scan(std::string_view filepath);

  Generator<std::expected<Token, int>> tokenize(std::string_view source_code);
  void run_prompt();

private:
  void eval_statement(std::string_view statement);
  std::vector<Token> m_tokens;
};
