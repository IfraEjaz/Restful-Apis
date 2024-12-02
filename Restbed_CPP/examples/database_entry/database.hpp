#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <map>
#include <variant>
#include <vector>    // Include vector header for storing lists of data
#include <sqlite3.h> // Include SQLite3 library for database operations
using namespace std;

// Define a variant type to handle multiple data types (int, double, string)
using DataType = std::variant<int, double, std::string>;

// Enum to represent results of insert operations
enum class InsertResult {
    SUCCESS,              // Insert successful
    EMPTY_DATA,           // No data provided for insertion
    TABLE_NOT_FOUND,      // Specified table does not exist
    COLUMN_NOT_FOUND,     // Specified column does not exist
    INVALID_DATA,         // Data does not match expected format or type
    QUERY_ERROR,          // General SQL query error
    CONNECTION_ERROR,     // Database connection issue
    PERMISSION_DENIED,    // User lacks permission for the operation
    DUPLICATE_ENTRY,      // Duplicate data entry detected
    DATA_TYPE_MISMATCH,   // Data type mismatch for column
    RECORD_NOT_FOUND      // No record matches the criteria
};

// Enum to represent results of update operations
enum class UpdateResult {
    SUCCESS,              // Update successful
    EMPTY_DATA,           // No data provided for update
    TABLE_NOT_FOUND,      // Specified table does not exist
    COLUMN_NOT_FOUND,     // Specified column does not exist
    INVALID_DATA,         // Data does not match expected format or type
    QUERY_ERROR,          // General SQL query error
    ROW_NOT_FOUND         // No matching row found for update
};

// Enum for errors when adding a column
enum class AddColumnError {
    None,                // No error
    InvalidColumnName,   // Provided column name is invalid
    ColumnAlreadyExists, // Column with the same name already exists
    SQLExecutionFailed   // SQL query execution failed
};

// Enum to represent results of delete operations
enum class DeleteResult {
    SUCCESS,              // Delete operation successful
    ID_NOT_FOUND,         // No record with the specified ID found
    SQL_EXECUTION_FAILED  // SQL query execution failed
};

// Database class declaration
class Database {
public:
    // Get a singleton instance of the Database class
    static Database& getInstance(const std::string& db_file);

    // Create a new table
    bool create_table(const std::string& table_name);

    // Insert data into a specified table
    InsertResult insert_data(const std::map<std::string, DataType>& data, const std::string& table_name);

    // Update data in a table based on the ID
    UpdateResult update_data(const string& table_name, int id, const map<string, DataType>& data);

    // Delete a record from a table by ID
    DeleteResult delete_data(const std::string& table_name, int id);

    // Add a new column to a table
    AddColumnError add_column(const std::string& table_name, const std::string& column_name, const std::string& data_type);

    // Delete a column from a table (note: SQLite doesn't support native column deletion)
    bool delete_column(const std::string& table_name);

    // Check if a column exists in a table
    bool column_exists(const std::string& table_name, const std::string& column_name);

    // Retrieve all column names from a table
    std::vector<std::string> get_table_columns(const std::string& table_name);

    // Retrieve all data from a table
    std::vector<map<string, DataType>> get_all_data(const string& table_name);

private:
    // Constructor to initialize database connection
    Database(const std::string& db_file);

    // Execute a SQL query without expecting results
    bool execute_sql(const std::string& sql);

    // Validate if a column name is syntactically valid
    bool is_valid_column_name(const string& column_name);

    // Execute a SQL query that returns a count value
    int execute_sql_query_for_count(const std::string& sql);

    // Helper functions to validate data types
    bool is_valid_int(const string& value);
    bool is_valid_float(const string& value);
    bool is_valid_string(const string& value);

    // Validate input data against expected column types
    bool validate_input(const string& col, const string& value, const map<string, string>& column_types);

    // Validate if a table name is syntactically valid
    bool is_valid_table_name(const string& table_name);

    // Convert a variant value to a SQL-compatible string
    std::string convert_variant_to_sql_value(const DataType& value);

    // SQLite3 database connection handle
    sqlite3* db = nullptr;

    // Destructor to close the database connection
    ~Database();
};

#endif // DATABASE_HPP
