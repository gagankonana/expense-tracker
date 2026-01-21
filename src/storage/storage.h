#ifndef SRC_STORAGE_STORAGE_H
#define SRC_STORAGE_STORAGE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include <chrono>
#include "../expense/expense.h"
#include "../expense/category.h"

class Storage {
public:
    explicit Storage(const std::string& db_path);
    ~Storage();

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

    bool initialize();

    // Category operations
    bool insertCategory(const Category& category);
    std::optional<Category> getCategoryByName(const std::string& name);
    std::optional<Category> getCategoryById(int id);
    std::vector<Category> getAllCategories();
    bool updateCategory(int id, const Category& category);
    bool deleteCategory(int id);
    std::optional<int> getCategoryId(const std::string& name);

    // Expense operations
    bool insertExpense(const Expense& expense);
    std::optional<Expense> getExpenseById(int id);
    std::vector<Expense> getAllExpenses();
    std::vector<Expense> getExpensesByCategory(const std::string& category_name);
    bool updateExpense(int id, const Expense& expense);
    bool deleteExpense(int id);

private:
    sqlite3* db_;
    std::string db_path_;

    bool createTables();
    int64_t timePointToUnix(const std::chrono::system_clock::time_point& tp);
    std::chrono::system_clock::time_point unixToTimePoint(int64_t unix_time);
};

#endif // SRC_STORAGE_STORAGE_H
