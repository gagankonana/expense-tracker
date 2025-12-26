#include "expense.h"

Expense* Expense::setAmount(const double amount) {
  amount_ = amount;
  return this;
}

Expense* Expense::setCategory(const Category &category) {
  category_ = category;
  return this;
}

Expense* Expense::setTimestamp(
    const std::chrono::system_clock::time_point &timestamp) {
  timestamp_ = timestamp;
  return this;
}

Expense* Expense::setDescription(const std::string &description) {
  description_ = description;
  return this;
}

double Expense::getAmount() const { return amount_; }
Category Expense::getCategory() const { return category_; }
std::chrono::system_clock::time_point Expense::getTimestamp() const {
  return timestamp_;
}
std::string_view Expense::getDescription() const { return description_; }