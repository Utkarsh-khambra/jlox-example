cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----
# Note: update this to your new project's name and version
project(
  crafting_interp
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.

# ---- Create library ----
# add_executable(crafting_interp src/impls/tests.cpp src/impls/catch_main.cpp)
add_executable(crafting_interp src/main.cpp)
# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(crafting_interp INTERFACE) set_target_properties(crafting_interp PROPERTIES
# INTERFACE_COMPILE_FEATURES cxx_std_17)

target_compile_features(crafting_interp PUBLIC cxx_std_20)
target_compile_options(crafting_interp PRIVATE -ggdb  -fsanitize=address -fsanitize=undefined -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused )

target_include_directories(
	crafting_interp PUBLIC $<BUILD_INTERFACE: ${fmt_INCLUDE_DIRS}  ${Catch2_INCLUDE_DIRS} >
)

target_link_libraries(crafting_interp PRIVATE -lasan -lubsan fmt::fmt Catch2::Catch2 )

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths

