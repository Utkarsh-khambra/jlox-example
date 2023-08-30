#include <fstream>
#include <utils.hpp>
[[nodiscard]] std::string read_file(std::filesystem::path file) noexcept {
  std::ifstream f(file.string());
  if (!f.is_open())
    throw std::runtime_error("Unable to read file");
}
