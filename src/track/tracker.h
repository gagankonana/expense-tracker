#ifndef SRC_TRACKER_H
#define SRC_TRACKER_H

#include "expense/category.h"
#include "expense/expense.h"
#include <vector>

class Tracker {
public:
    Tracker() = default;
   
    Expense createExpense(double amount, const Category &category,
                       const std::chrono::system_clock::time_point &timestamp,
                       const std::string &description, const std::string &title);
    void addExpense(const Expense& expense);
    void updateExpense(size_t index, const std::string& field, const std::string& value);
    void removeExpense(size_t index);

    std::vector<Expense> getExpensesForYear(int year) const;
    std::vector<Expense> getExpensesForMonth(int month, int year) const;
    // will be that day + 6 days
    std::vector<Expense> getExpensesForWeek(int day, int month, int year) const;
    std::vector<Expense> getExpensesForDay(int day, int month, int year) const;

    void createCategory(const std::string &name, const std::optional<std::string> &description = nullptr);
    void deleteCategory(const std::string& name);
    void updateCategoryName(const std::string& oldName, const std::string& newName);
    void updateCategoryDescription(const std::string& name, const std::optional<std::string>& newDescription);  
    std::vector<Category> getCategories() const;

private:
    std::vector<Expense> expenses;
};

#endif // SRC_TRACKER_H