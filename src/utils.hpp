#pragma once
#include <filesystem>
#include <string>

[[nodiscard]] constexpr std::string
read_file(std::filesystem::path file) noexcept;
