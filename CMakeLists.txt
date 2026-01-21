cmake_minimum_required(VERSION 3.10)

project(ExpenseTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Source files
set(SOURCES
    main.cpp
    src/expense/expense.cpp
    src/storage/storage.cpp
)

# Add the executable
add_executable(expense_tracker ${SOURCES})

# Include directories
target_include_directories(expense_tracker PRIVATE src)

# Link SQLite3
target_link_libraries(expense_tracker PRIVATE SQLite::SQLite3)
