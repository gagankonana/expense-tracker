#include "expense.h"


Expense::Expense(double amount, const Category &category,
  const std::string &description, const std::string &title) {
  amount_ = amount;
  category_ = category;
  description_ = description;
  title_ = title;
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);
  date_ = utils::Date{tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900};
}

Expense* Expense::setAmount(const double amount) {
  amount_ = amount;
  return this;
}

Expense* Expense::setCategory(const Category &category) {
  category_ = category;
  return this;
}

Expense* Expense::setDate(
    const utils::Date &date) {
  date_ = date;
  return this;
}

Expense* Expense::setDescription(const std::string &description) {
  description_ = description;
  return this;
}

 std::string Expense::getExpenseSummary() {
  return "Description: " + description_ + ", Amount: " + std::to_string(amount_) +
         ", Category: " + category_.getName();
}