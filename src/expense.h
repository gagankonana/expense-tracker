#ifndef SRC_TRACKER_EXPENSE_H
#define SRC_TRACKER_EXPENSE_H

#include "category.h"
#include "chrono"
#include "ctime"
#include "string"
#include <string_view>

class Category;

class Expense {
public:
  Expense() = default;
  Expense(double amount, const Category &category,
          const std::chrono::system_clock::time_point &timestamp,
          const std::string &description)
      : amount_(amount), category_(category), timestamp_(timestamp),
        description_(description) {}
    
    Expense* setAmount(double amount);
    Expense* setCategory(const Category &category);
    Expense* setTimestamp(const std::chrono::system_clock::time_point &timestamp);
    Expense* setDescription(const std::string &description);

    double getAmount() const;
    Category getCategory() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string_view getDescription() const;

private:
  double amount_;
  Category category_;
  std::chrono::system_clock::time_point timestamp_;
  std::string description_;
};

#endif