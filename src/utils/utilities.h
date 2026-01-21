#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>

namespace utils {

enum Operation {
    ADD_EXPENSE,
    VIEW_EXPENSES,
    ADD_CATEGORY,
    VIEW_CATEGORIES,
    // UPDATE_EXPENSE,
    // DELETE_EXPENSE,
    // UPDATE_CATEGORY,
    // DELETE_CATEGORY,
    INVALID_OPERATION
};

struct Command {
    Operation operation;
    std::vector<std::string> arguments;
};

struct Date {
    int day;
    int month;
    int year;
};
/*
examples
- create category "Food" "Expenses for food"
- add expense 15.50 "Food" "2024-06-15" "Lunch at cafe" "Lunch"
- view expenses --category "Food" --from "2024-06-01" --to "2024-06-30"
- view categories
- view expenses
- view category "Food"
*/ 
Command parseCommandLineArgs(int argc, char* argv[]);

}

#endif // UTILITIES_H