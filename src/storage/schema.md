# Database Schema

## Tables

### category

| Column | Type | Constraints |
|--------|------|-------------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT |
| name | TEXT | NOT NULL, UNIQUE |
| description | TEXT | |

### expense

| Column | Type | Constraints |
|--------|------|-------------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT |
| amount | REAL | NOT NULL |
| category_id | INTEGER | FOREIGN KEY → category(id) |
| timestamp | INTEGER | NOT NULL (Unix timestamp) |
| description | TEXT | |
| title | TEXT | |
