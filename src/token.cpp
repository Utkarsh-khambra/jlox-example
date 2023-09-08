#include "token.hpp"
#include <utility>

LineOffset operator++(LineOffset line) noexcept {
  return LineOffset{std::to_underlying(line) + 1};
}
