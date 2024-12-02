#include "database.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <algorithm> // for std::find
#include <regex> // For std::regex and std::regex_match
#include <set>   // For std::set


using namespace std;
// Placeholder for reserved SQL keywords
const set<string> RESERVED_SQL_KEYWORDS = {"SELECT", "TABLE", "WHERE", "FROM", "INSERT", "DELETE", "UPDATE", "JOIN", "ORDER", "GROUP"};


// Constructor for Database class
Database::Database(const string& db_file) {
    if (sqlite3_open(db_file.c_str(), &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        db = nullptr;
    } else {
        cout << "Connected to database successfully: " << db_file << endl;
    }
}

// Destructor for Database class
Database::~Database() {
    if (db != nullptr) {
        sqlite3_close(db);
        cout << "Database connection closed." << endl;
    }
}

// Singleton pattern to ensure a single database connection
Database& Database::getInstance(const string& db_file) {
    static Database instance(db_file);
    return instance;
}

// Execute SQL statement
bool Database::execute_sql(const string& sql) {
    char* err_msg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);

    if (result != SQLITE_OK) {
        cerr << "SQL error: " << err_msg << endl;
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

int Database::execute_sql_query_for_count(const std::string& sql) {
    sqlite3_stmt* stmt;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to execute count query: " << sqlite3_errmsg(db) << std::endl;
    }

    return count;
}

// Function to validate database or table names
bool Database::is_valid_table_name(const string& table_name) {
    // Check if the table name matches the pattern: starts with a capital letter and only contains alphanumeric characters or underscores
    if (!regex_match(table_name, regex("^[A-Z][A-Za-z0-9_]*$"))) {
        cerr << "Invalid table name format: " << table_name << endl;
        return false;
    }

    // Check if the table name is not a reserved keyword
    if (RESERVED_SQL_KEYWORDS.count(table_name)) {
        cerr << "Table name cannot be a reserved SQL keyword: " << table_name << endl;
        return false;
    }

    // Check if the table name does not exceed a certain word limit (e.g., maximum 3 words)
    if (count(table_name.begin(), table_name.end(), '_') > 2) {
        cerr << "Table name exceeds the maximum allowed word limit: " << table_name << endl;
        return false;
    }

    return true;
}

// Function to create a table with validation
bool Database::create_table(const string& table_name) {
    // Validate the table name
    if (!is_valid_table_name(table_name)) {
        cerr << "Failed to create table due to invalid table name: " << table_name << endl;
        return false;
    }

    // Construct SQL for creating the table
    string sql = "CREATE TABLE IF NOT EXISTS " + table_name + "(id INTEGER PRIMARY KEY AUTOINCREMENT);";
    return execute_sql(sql);
}


// Function to check existence of a column
bool Database::column_exists(const string& table_name, const string& column_name) {
    string sql = "PRAGMA table_info(" + table_name + ");";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        cerr << "Failed to fetch table info: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string col_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (col_name == column_name) {
            sqlite3_finalize(stmt);
            return true;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}
//Function to check if a column name  is  valid
bool Database::is_valid_column_name(const string& column_name)
{// Check if the column name starts with a capital letter
    if (!isupper(column_name[0])) {
        cerr << "Error: Column name must start with a capital letter." << endl;
        return false;
    }

    // Check if the column name starts with a letter
    if (!isalpha(column_name[0])) {
        cerr << "Error: Column name must start with a letter." << endl;
        return false;
    }

    // Check for special characters (only allow alphanumeric and underscore)
    if (!regex_match(column_name, regex("^[A-Za-z][A-Za-z0-9_]*$"))) {
        cerr << "Error: Column name can only contain alphanumeric characters and underscores." << endl;
        return false;
    }

    // Check for leading or trailing underscores
    if (column_name.front() == '_' || column_name.back() == '_') {
        cerr << "Error: Column name cannot start or end with an underscore." << endl;
        return false;
    }

    // Check for multiple consecutive underscores
    if (column_name.find("__") != string::npos) {
        cerr << "Error: Column name cannot have consecutive underscores." << endl;
        return false;
    }

    // Check if the column name contains spaces
    if (column_name.find(' ') != string::npos) {
        cerr << "Error: Column name cannot contain spaces." << endl;
        return false;
    }

    // Check if the column name is a reserved SQL keyword
    if (RESERVED_SQL_KEYWORDS.count(column_name)) {
        cerr << "Error: Column name cannot be a reserved SQL keyword." << endl;
        return false;
    }

    // Check the length limit (e.g., max 30 characters)
    if (column_name.length() > 30) {
        cerr << "Error: Column name cannot exceed 30 characters." << endl;
        return false;
    }

    // Check for the maximum number of words (3 words, assuming underscores are used as separators)
    size_t word_count = count(column_name.begin(), column_name.end(), '_') + 1;
    if (word_count > 3) {
        cerr << "Error: Column name cannot exceed three words." << endl;
        return false;
    }

    // If all checks pass, return true
    return true;
}
//Function to add columns in database
AddColumnError Database::add_column(const string& table_name, const string& column_name, const string& data_type) {
    // Validate the column name
    if (!is_valid_column_name(column_name)) {
        cerr << "Invalid column name: " << column_name << endl;
        return AddColumnError::InvalidColumnName;
    }

    // Check if the column already exists
    if (column_exists(table_name, column_name)) {
        cerr << "Column '" << column_name << "' already exists in table '" << table_name << "'." << endl;
        return AddColumnError::ColumnAlreadyExists;
    }

    // Attempt to add the column
    string sql = "ALTER TABLE " + table_name + " ADD COLUMN " + column_name + " " + data_type + ";";
    if (!execute_sql(sql)) {
        cerr << "Failed to execute SQL for adding column: " << column_name << endl;
        return AddColumnError::SQLExecutionFailed;
    }

    return AddColumnError::None; // No errors
}


// Convert variant to SQL value
string Database::convert_variant_to_sql_value(const DataType& value) {
    if (holds_alternative<int>(value)) {
        return to_string(get<int>(value));
    } else if (holds_alternative<double>(value)) {
        return to_string(get<double>(value));
    } else if (holds_alternative<string>(value)) {
        return "'" + get<string>(value) + "'";
    }
    return "NULL";
}

// Function to get table columns for comparing with input data columns to put or update data 
vector<string> Database::get_table_columns(const string& table_name) {
    vector<string> columns;
    string sql = "PRAGMA table_info(" + table_name + ");";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (colName) {
                columns.push_back(string(colName));
            }
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to retrieve table columns." << endl;
    }

    return columns;
}

// Input validation functions
bool Database::is_valid_int(const string& value) {
    try {
        int val = stoi(value);
        return (val >= 0 && val <= 9999);
    } catch (...) {
        return false;
    }
}

bool Database::is_valid_float(const string& value) {
    try {
        float val = stof(value);
        return (val >= 0.0 && val <= 99);
    } catch (...) {
        return false;
    }
}

bool Database::is_valid_string(const string& value) {
    return value.length() <= 255;
}

// Function to validate the data type of each input value
bool Database::validate_input(const string& col, const string& value, const map<string, string>& column_types) {
    if (column_types.find(col) != column_types.end()) {
        string type = column_types.at(col);

        if (type == "INTEGER") {
            return is_valid_int(value);
        } else if (type == "REAL") {
            return is_valid_float(value);
        } else if (type == "STRING") {
            return is_valid_string(value);
        }
    }
    return false; // If column type is unknown or value is invalid
}

// Insert data
InsertResult Database::insert_data(const map<string, DataType>& data, const string& table_name) {
    if (data.empty()) {
         return  InsertResult::EMPTY_DATA;
    }

    vector<string> existing_columns = get_table_columns(table_name);
    if (existing_columns.empty()) {
        cerr << "No columns found for the table or failed to retrieve columns from the table." << endl;
       return InsertResult::TABLE_NOT_FOUND;
    }

    map<string, string> column_types;
    string sql = "PRAGMA table_info(" + table_name + ");";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* colType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            if (colName && colType) {
                column_types[string(colName)] = string(colType);
            }
        }
        sqlite3_finalize(stmt);
    }

    // Check if the input columns match with the table columns
    for (const auto& [col, val] : data) {
        if (find(existing_columns.begin(), existing_columns.end(), col) == existing_columns.end()) {
            cerr << "Column '" << col << "' does not exist in the table '" << table_name << "'." << endl;
            return InsertResult::COLUMN_NOT_FOUND;
        }
        // Convert variant type to string
        string value_str;
        if (holds_alternative<int>(val)) {
            value_str = to_string(get<int>(val));
        } else if (holds_alternative<double>(val)) {
            value_str = to_string(get<double>(val));
        } else if (holds_alternative<string>(val)) {
            value_str = get<string>(val);
        } else {
            value_str = "NULL"; // Or handle accordingly
        }

        // Validate the data type for this column
        if (!validate_input(col, value_str, column_types)) {
            cerr << "Invalid data for column '" << col << "'. Value: ";
            visit([](auto&& arg) { cerr << arg; }, val);  // val is a std::variant
            cerr << endl;
             return InsertResult::QUERY_ERROR;
        }
    }

    // Prepare SQL INSERT statement
    sql = "INSERT INTO " + table_name + " (";
    string columns;
    string values;

    for (const auto& [col, val] : data) {
        if (!columns.empty()) {
            columns += ",";
            values += ",";
        }
        columns += col;
        values += convert_variant_to_sql_value(val);
    }

    sql += columns + ") VALUES (" + values + ");";
    bool sql_result = execute_sql(sql);

    if (sql_result) {
        return InsertResult::SUCCESS;
    } else {
        return InsertResult::QUERY_ERROR;
    }
}
//Update data  
UpdateResult Database::update_data(const string& table_name, int id, const map<string, DataType>& data) {
    if (data.empty()) {
        return UpdateResult::EMPTY_DATA;
    }

    // Retrieve existing columns from the table
    vector<string> existing_columns = get_table_columns(table_name);
    if (existing_columns.empty()) {
        cerr << "No columns found for the table or failed to retrieve columns from the table." << endl;
        return UpdateResult::TABLE_NOT_FOUND;
    }

    map<string, string> column_types;
    string sql = "PRAGMA table_info(" + table_name + ");";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* colType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            if (colName && colType) {
                column_types[string(colName)] = string(colType);
            }
        }
        sqlite3_finalize(stmt);
    }

    // Check if the input columns match with the table columns
    for (const auto& [col, val] : data) {
        if (find(existing_columns.begin(), existing_columns.end(), col) == existing_columns.end()) {
            cerr << "Column '" << col << "' does not exist in the table '" << table_name << "'." << endl;
            return UpdateResult::COLUMN_NOT_FOUND;
        }

        string value_str;
        if (holds_alternative<int>(val)) {
            value_str = to_string(get<int>(val));
        } else if (holds_alternative<double>(val)) {
            value_str = to_string(get<double>(val));
        } else if (holds_alternative<string>(val)) {
            value_str = get<string>(val);
        } else {
            value_str = "NULL"; 
        }

        if (!validate_input(col, value_str, column_types)) {
            cerr << "Invalid data for column '" << col << "'. Value: ";
            visit([](auto&& arg) { cerr << arg; }, val); 
            cerr << endl;
            return UpdateResult::INVALID_DATA;
        }
    }

    // Prepare SQL UPDATE statement
    sql = "UPDATE " + table_name + " SET ";
    string updates;

    for (const auto& [col, val] : data) {
        if (!updates.empty()) {
            updates += ", ";
        }
        updates += col + " = " + convert_variant_to_sql_value(val);
    }

    sql += updates + " WHERE id = " + to_string(id) + ";";

    char* errmsg;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        cerr << "Failed to execute update query: " << sql << " Error: " << errmsg << endl;
        sqlite3_free(errmsg);
        return UpdateResult::QUERY_ERROR;
    }

    // Check number of affected rows
    if (sqlite3_changes(db) == 0) {
        cerr << "No rows were updated. ID may not exist in the table." << endl;
        return UpdateResult::ROW_NOT_FOUND; // Add this to UpdateResult enum
    }

    return UpdateResult::SUCCESS;
}

// Function to delete data 

DeleteResult Database::delete_data(const string& table_name, int id) {
    // Check if the ID exists in the table
    string check_sql = "SELECT COUNT(*) FROM " + table_name + " WHERE id = " + to_string(id) + ";";
    int count = execute_sql_query_for_count(check_sql);

    if (count == 0) {
        // ID does not exist
        return DeleteResult::ID_NOT_FOUND;
    }

    // ID exists; proceed with delete operation
    string delete_sql = "DELETE FROM " + table_name + " WHERE id = " + to_string(id) + ";";
    if (execute_sql(delete_sql)) {
        return DeleteResult::SUCCESS;
    }

    // SQL execution failed
    return DeleteResult::SQL_EXECUTION_FAILED;
}
// function to get all data  
vector<map<string, DataType>> Database::get_all_data(const string& table_name) {
    vector<map<string, DataType>> results;
    string sql = "SELECT * FROM " + table_name + ";";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            map<string, DataType> row;
            int col_count = sqlite3_column_count(stmt);
            for (int col = 0; col < col_count; ++col) {
                string col_name = reinterpret_cast<const char*>(sqlite3_column_name(stmt, col));

                switch (sqlite3_column_type(stmt, col)) {
                    case SQLITE_INTEGER:
                        row[col_name] = sqlite3_column_int(stmt, col);
                        break;
                    case SQLITE_FLOAT:
                        row[col_name] = sqlite3_column_double(stmt, col);
                        break;
                    case SQLITE_TEXT:
                        row[col_name] = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, col)));
                        break;
                    case SQLITE_NULL:
                        row[col_name] = nullptr; // Handle NULL case
                        break;
                    default:
                        row[col_name] = ""; // Default to empty string
                }
            }
            results.push_back(row);
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to retrieve data from the table." << endl;
    }

    return results;
}
