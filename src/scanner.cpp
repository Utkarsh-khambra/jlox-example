#include "scanner.hpp"
#include <iostream>
#include <fmt/core.h>
void Scanner::scan(std::string_view file){

}

void Scanner::run_prompt() {
    std::string current_line;
    while(std::getline(std::cin, current_line))
    {
        eval_statement(current_line);
    }
}

void Scanner::eval_statement(std::string_view statement){

        fmt::print("{}\n", statement);
}

