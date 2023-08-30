#pragma once
#include <string_view>

class Scanner {
public:
  void scan(std::string_view text);

  void run_prompt();

private:
  void eval_statement(std::string_view statement);
};
