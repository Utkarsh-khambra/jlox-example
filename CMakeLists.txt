cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----
# Note: update this to your new project's name and version
project(
  jlox
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_subdirectory(src)

add_subdirectory(test)
add_dependencies(tests jlox)

