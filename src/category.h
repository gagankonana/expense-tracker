#ifndef SRC_CATEGORY_H
#define SRC_CATEGORY_H

#include <optional>
#include <string>

class Category {
public:
  Category() = default;
  explicit Category(const std::string &name) : name_(name) {}
  explicit Category(const std::string &name, const std::string &description)
      : name_(name), description_(description) {}

  std::string getName() const { return name_; }
  void setName(const std::string &name) { name_ = name; }

private:
  std::string name_;
  std::optional<std::string> description_;
};

#endif // SRC_CATEGORY_H