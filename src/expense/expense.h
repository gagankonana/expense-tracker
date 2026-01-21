#ifndef SRC_TRACKER_EXPENSE_H
#define SRC_TRACKER_EXPENSE_H

#include "category.h"
#include "chrono"
#include "string"
#include <string_view>
#include "utils/utilities.h"

class Category;

class Expense {
public:
    Expense() = default;
    Expense(double amount, const Category &category,
            utils::Date &date,
            const std::string &description, const std::string &title)
        : amount_(amount), category_(category), date_(date),
          description_(description), title_(title) {}

    Expense(double amount, const Category &category,
          const std::string &description, const std::string &title);
    
    Expense* setAmount(double amount);
    Expense* setCategory(const Category &category);
    Expense* setDate(const std::chrono::system_clock::time_point &timestamp);
    Expense* setDescription(const std::string &description);
    Expense* setTitle(const std::string &title);

    double getAmount() const { return amount_; };
    Category getCategory() const { return category_; };
    utils::Date getDate() const { return date_; };
    std::string_view getDescription() const { return description_; };
    std::string_view getTitle() const { return title_; };

    std::string getExpenseSummary();

private:
  double amount_;
  Category category_;
  utils::Date date_;
  std::string description_;
  std::string title_;
};

#endif