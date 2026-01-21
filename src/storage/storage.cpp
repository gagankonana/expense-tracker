#include "storage.h"
#include <iostream>

Storage::Storage(const std::string& db_path) : db_(nullptr), db_path_(db_path) {}

Storage::~Storage() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool Storage::initialize() {
    int rc = sqlite3_open(db_path_.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    return createTables();
}

bool Storage::createTables() {
    const char* category_table = R"(
        CREATE TABLE IF NOT EXISTS category (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            description TEXT
        );
    )";

    const char* expense_table = R"(
        CREATE TABLE IF NOT EXISTS expense (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            amount REAL NOT NULL,
            category_id INTEGER,
            timestamp INTEGER NOT NULL,
            description TEXT,
            title TEXT,
            FOREIGN KEY (category_id) REFERENCES category(id)
        );
    )";

    char* err_msg = nullptr;

    int rc = sqlite3_exec(db_, category_table, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create category table: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    rc = sqlite3_exec(db_, expense_table, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create expense table: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    return true;
}

int64_t Storage::timePointToUnix(const std::chrono::system_clock::time_point& tp) {
    return std::chrono::duration_cast<std::chrono::seconds>(
        tp.time_since_epoch()).count();
}

std::chrono::system_clock::time_point Storage::unixToTimePoint(int64_t unix_time) {
    return std::chrono::system_clock::time_point(std::chrono::seconds(unix_time));
}

// Category operations
bool Storage::insertCategory(const Category& category) {
    const char* sql = "INSERT INTO category (name, description) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, category.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_null(stmt, 2); // description not exposed in Category getter

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

std::optional<int> Storage::getCategoryId(const std::string& name) {
    const char* sql = "SELECT id FROM category WHERE name = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    std::optional<int> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return result;
}

std::optional<Category> Storage::getCategoryByName(const std::string& name) {
    const char* sql = "SELECT id, name, description FROM category WHERE name = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    std::optional<Category> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* cat_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        if (desc) {
            result = Category(cat_name, desc);
        } else {
            result = Category(cat_name);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

std::optional<Category> Storage::getCategoryById(int id) {
    const char* sql = "SELECT id, name, description FROM category WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, id);

    std::optional<Category> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        if (desc) {
            result = Category(name, desc);
        } else {
            result = Category(name);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Category> Storage::getAllCategories() {
    std::vector<Category> categories;
    const char* sql = "SELECT id, name, description FROM category;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return categories;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        if (desc) {
            categories.emplace_back(name, desc);
        } else {
            categories.emplace_back(name);
        }
    }

    sqlite3_finalize(stmt);
    return categories;
}

bool Storage::updateCategory(int id, const Category& category) {
    const char* sql = "UPDATE category SET name = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, category.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Storage::deleteCategory(int id) {
    const char* sql = "DELETE FROM category WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

// Expense operations
bool Storage::insertExpense(const Expense& expense) {
    auto category_id = getCategoryId(expense.getCategory().getName());
    if (!category_id.has_value()) {
        // Insert category first if it doesn't exist
        if (!insertCategory(expense.getCategory())) {
            return false;
        }
        category_id = getCategoryId(expense.getCategory().getName());
    }

    const char* sql = "INSERT INTO expense (amount, category_id, timestamp, description, title) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, expense.getAmount());
    sqlite3_bind_int(stmt, 2, category_id.value());
    sqlite3_bind_int64(stmt, 3, timePointToUnix(expense.getTimestamp()));
    sqlite3_bind_text(stmt, 4, std::string(expense.getDescription()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, std::string(expense.getTitle()).c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

std::optional<Expense> Storage::getExpenseById(int id) {
    const char* sql = R"(
        SELECT e.id, e.amount, e.timestamp, e.description, e.title, c.name, c.description
        FROM expense e
        LEFT JOIN category c ON e.category_id = c.id
        WHERE e.id = ?;
    )";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, id);

    std::optional<Expense> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double amount = sqlite3_column_double(stmt, 1);
        int64_t timestamp = sqlite3_column_int64(stmt, 2);
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* cat_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* cat_desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        Category cat = cat_desc ? Category(cat_name ? cat_name : "", cat_desc)
                                : Category(cat_name ? cat_name : "");

        result = Expense(amount, cat, unixToTimePoint(timestamp),
                        desc ? desc : "", title ? title : "");
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Expense> Storage::getAllExpenses() {
    std::vector<Expense> expenses;
    const char* sql = R"(
        SELECT e.id, e.amount, e.timestamp, e.description, e.title, c.name, c.description
        FROM expense e
        LEFT JOIN category c ON e.category_id = c.id;
    )";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return expenses;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double amount = sqlite3_column_double(stmt, 1);
        int64_t timestamp = sqlite3_column_int64(stmt, 2);
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* cat_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* cat_desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        Category cat = cat_desc ? Category(cat_name ? cat_name : "", cat_desc)
                                : Category(cat_name ? cat_name : "");

        expenses.emplace_back(amount, cat, unixToTimePoint(timestamp),
                             desc ? desc : "", title ? title : "");
    }

    sqlite3_finalize(stmt);
    return expenses;
}

std::vector<Expense> Storage::getExpensesByCategory(const std::string& category_name) {
    std::vector<Expense> expenses;
    const char* sql = R"(
        SELECT e.id, e.amount, e.timestamp, e.description, e.title, c.name, c.description
        FROM expense e
        LEFT JOIN category c ON e.category_id = c.id
        WHERE c.name = ?;
    )";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return expenses;
    }

    sqlite3_bind_text(stmt, 1, category_name.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double amount = sqlite3_column_double(stmt, 1);
        int64_t timestamp = sqlite3_column_int64(stmt, 2);
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* cat_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* cat_desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        Category cat = cat_desc ? Category(cat_name ? cat_name : "", cat_desc)
                                : Category(cat_name ? cat_name : "");

        expenses.emplace_back(amount, cat, unixToTimePoint(timestamp),
                             desc ? desc : "", title ? title : "");
    }

    sqlite3_finalize(stmt);
    return expenses;
}

bool Storage::updateExpense(int id, const Expense& expense) {
    auto category_id = getCategoryId(expense.getCategory().getName());
    if (!category_id.has_value()) {
        if (!insertCategory(expense.getCategory())) {
            return false;
        }
        category_id = getCategoryId(expense.getCategory().getName());
    }

    const char* sql = "UPDATE expense SET amount = ?, category_id = ?, timestamp = ?, description = ?, title = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_double(stmt, 1, expense.getAmount());
    sqlite3_bind_int(stmt, 2, category_id.value());
    sqlite3_bind_int64(stmt, 3, timePointToUnix(expense.getTimestamp()));
    sqlite3_bind_text(stmt, 4, std::string(expense.getDescription()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, std::string(expense.getTitle()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Storage::deleteExpense(int id) {
    const char* sql = "DELETE FROM expense WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}
