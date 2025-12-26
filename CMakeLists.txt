cmake_minimum_required(VERSION 3.10)

project(ExpenseTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(expense_tracker main.cpp)

# Include directories
target_include_directories(expense_tracker PRIVATE src)

# If there are source files in src, add them here
# For now, assuming headers only, but if there are .cpp files, add them to add_executable or create a library

# Example if there are source files:
# file(GLOB SOURCES "src/*.cpp")
# add_executable(expense_tracker main.cpp ${SOURCES})
# target_include_directories(expense_tracker PRIVATE src)