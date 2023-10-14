#pragma once
#include "token.hpp"
#include <expected>
#include <generator.hpp>
#include <string_view>
#include <vector>

enum class ScanError {
  NoString,           // Invalid string
  NoMultiLineComment, // Invalid multiline comment
  NoScanableToken
};

class Scanner {
public:
  void scan(std::string_view filepath);

  Generator<std::expected<Token, ScanError>>
  tokenize(std::string_view source_code);
  void run_prompt();

private:
  void eval_statement(std::string_view statement);
  std::vector<Token> m_tokens;
};
