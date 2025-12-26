#ifndef SRC_TRACKER_H
#define SRC_TRACKER_H

#include "category.h"
#include "expense.h"
#include <vector>

class Tracker {
public:
    Tracker() = default;
   
    Expense createExpense(double amount, const Category &category,
                       const std::chrono::system_clock::time_point &timestamp,
                       const std::string &description);
    void addExpense(const Expense& expense);
    void updateExpense(size_t index, const std::string& field, const std::string& value);
    void removeExpense(size_t index);
    std::vector<std::shared_ptr<Expense>> getExpenses() const;

    void createCategory(const std::string &name, const std::optional<std::string> &description = nullptr);
    void deleteCategory(const std::string& name);
    void updateCategoryName(const std::string& oldName, const std::string& newName);
    void updateCategoryDescription(const std::string& name, const std::optional<std::string>& newDescription);  
    std::vector<std::shared_ptr<Category>> getCategories() const;

private:
    std::vector<std::shared_ptr<Expense>> expenses;
};

#endif // SRC_TRACKER_H