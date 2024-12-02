#include <restbed>
#include "database.hpp"
#include <memory>
#include <string>
#include <iostream>
#include <variant>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;
using namespace restbed;
// Handler functio nto create table
void create_table_handler(const shared_ptr<Session> session)
{
    const auto request=session->get_request();
    string db_name=request->get_query_parameter("db_name");
    string table_name=request->get_query_parameter("table_name");
    Database& db=Database::getInstance(db_name);
    if(db.create_table(table_name))
    {
        session->close(OK,"Table created successfully.",{{"Content-Type","application/json"}});
    }
    else
    {
        session->close(INTERNAL_SERVER_ERROR,"Failed to create a table.",{{"Content-Type","application/json"}});
    }
}
// Handler function to add column 
void add_column_handler(const shared_ptr<Session> session) {
    const auto request = session->get_request();
    string db_name = request->get_query_parameter("db_name");
    string table_name = request->get_query_parameter("table_name");
    string column_name = request->get_query_parameter("column_name");
    string data_type = request->get_query_parameter("data_type");

    Database& db = Database::getInstance(db_name);
    AddColumnError error = db.add_column(table_name, column_name, data_type);

    switch (error) {
        case AddColumnError::None:
            session->close(OK, R"({"message": "Column added successfully."})", {{"Content-Type", "application/json"}});
            break;

        case AddColumnError::InvalidColumnName:
            session->close(BAD_REQUEST, R"({"error": "Invalid column name."})", {{"Content-Type", "application/json"}});
            break;

        case AddColumnError::ColumnAlreadyExists:
            session->close(CONFLICT, R"({"error": "Column already exists."})", {{"Content-Type", "application/json"}});
            break;

        case AddColumnError::SQLExecutionFailed:
            session->close(INTERNAL_SERVER_ERROR, R"({"error": "SQL execution failed."})", {{"Content-Type", "application/json"}});
            break;
    }
}

// Handler functio to insert data 
void insert_data_handler(const shared_ptr<Session> session) {
    try {
        const auto request = session->get_request();
        string db_name = request->get_query_parameter("db_name");
        string table_name = request->get_query_parameter("table_name");

        int content_length = request->get_header("Content-Length", 0);
        session->fetch(content_length, [db_name, table_name](const shared_ptr<Session> session, const Bytes &body) {
            Database& db = Database::getInstance(db_name);
            
            // Parse and log the request body
            string body_str = string(body.begin(), body.end());
            cout << "Received body: " << body_str << endl;

            map<string, DataType> data;
            istringstream stream(body_str);
            string key_value_pair;
            while (getline(stream, key_value_pair, '&')) {
                size_t delimiter_pos = key_value_pair.find('=');
                if (delimiter_pos != string::npos) {
                    string key = key_value_pair.substr(0, delimiter_pos);
                    string value = key_value_pair.substr(delimiter_pos + 1);
                    cout << "Parsed key: " << key << ", value: " << value << endl;
                    data[key] = value;
                }
            }

            InsertResult result = db.insert_data(data, table_name);
            if (result == InsertResult::SUCCESS) {
                session->close(OK, "{\"message\": \"Data inserted successfully.\"}", {{"Content-Type", "application/json"}});
            } else {
                string error_message;
                switch (result) {
                    case InsertResult::EMPTY_DATA:
                        error_message = "Data is empty.";
                        break;
                    case InsertResult::TABLE_NOT_FOUND:
                        error_message = "Table not found.";
                        break;
                    case InsertResult::COLUMN_NOT_FOUND:
                        error_message = "Column not found.";
                        break;
                    case InsertResult::INVALID_DATA:
                        error_message = "Invalid data type.";
                        break;
                    case InsertResult::QUERY_ERROR:
                        error_message = "Query execution failed.";
                        break;
                    default:
                        error_message = "Unknown error.";
                        break;
                }
                cerr << "Insert data error: " << error_message << endl;  // Log the error
                string json_error = "{\"error\": \"" + error_message + "\"}";
                session->close(INTERNAL_SERVER_ERROR, json_error, {{"Content-Type", "application/json"}});
            }
        });
    } catch (const exception& e) {
        cerr << "Exception in insert_data_handler: " << e.what() << endl;
        string json_error = "{\"error\": \"Exception occurred: " + string(e.what()) + "\"}";
        session->close(INTERNAL_SERVER_ERROR, json_error, {{"Content-Type", "application/json"}});
    }
}

//// Handler function to update data 
void update_data_handler(const shared_ptr<Session> session) {
    

    try {
    //////////
    try {
        const auto request = session->get_request();
        string db_name = request->get_query_parameter("db_name");
        string table_name = request->get_query_parameter("table_name");
        int id = stoi(request->get_query_parameter("id"));

        int content_length = request->get_header("Content-Length", 0);
        session->fetch(content_length, [db_name, table_name, id](const shared_ptr<Session> session, const Bytes& body) {
            Database& db = Database::getInstance(db_name);
            
            // Parse request body
            string body_str = string(body.begin(), body.end());
            cout << "Received body: " << body_str << endl;

            map<string, DataType> data;
            istringstream stream(body_str);
            string key_value_pair;
            while (getline(stream, key_value_pair, '&')) {
                size_t delimiter_pos = key_value_pair.find('=');
                if (delimiter_pos != string::npos) {
                    string key = key_value_pair.substr(0, delimiter_pos);
                    string value = key_value_pair.substr(delimiter_pos + 1);
                    data[key] = value;
                }
            }

            UpdateResult result = db.update_data(table_name, id, data);
            if (result == UpdateResult::SUCCESS) {
    session->close(OK, "{\"message\": \"Data updated successfully.\"}", {{"Content-Type", "application/json"}});
} else {
    int status_code;
    string error_message;

    switch (result) {
         case UpdateResult::ROW_NOT_FOUND:
            status_code = NOT_FOUND;  // 404
            error_message = "ID not found in the table.";
            break;
        case UpdateResult::EMPTY_DATA:
            status_code = BAD_REQUEST;  // 400
            error_message = "No data provided.";
            break;
        case UpdateResult::TABLE_NOT_FOUND:
            status_code = NOT_FOUND;  // 404
            error_message = "Table not found.";
            break;
        case UpdateResult::COLUMN_NOT_FOUND:
            status_code = BAD_REQUEST;  // 400
            error_message = "Column not found.";
            break;
        case UpdateResult::INVALID_DATA:
            status_code = UNPROCESSABLE_ENTITY;  // 422
            error_message = "Invalid data type.";
            break;
        case UpdateResult::QUERY_ERROR:
            status_code = INTERNAL_SERVER_ERROR;  // 500
            error_message = "Query execution failed.";
            break;
       

        default:
            status_code = INTERNAL_SERVER_ERROR;  // 500
            error_message = "Unknown error.";
            break;
    }

    string json_error = "{\"error\": \"" + error_message + "\"}";
    session->close(status_code, json_error, {{"Content-Type", "application/json"}});
}

        });
    } catch (const exception& e) {
        string json_error = "{\"error\": \"Exception occurred: " + string(e.what()) + "\"}";
        session->close(INTERNAL_SERVER_ERROR, json_error, {{"Content-Type", "application/json"}});
    }



} catch (const exception& e) {
    cerr << "Exception occurred: " << e.what() << endl;
    string json_error = "{\"error\": \"Internal exception: " + string(e.what()) + "\"}";
    session->close(INTERNAL_SERVER_ERROR, json_error, {{"Content-Type", "application/json"}});
} catch (...) {
    cerr << "Unknown exception occurred." << endl;
    string json_error = "{\"error\": \"An unknown error occurred.\"}";
    session->close(INTERNAL_SERVER_ERROR, json_error, {{"Content-Type", "application/json"}});
}

}


//Delete data handler
void delete_data_handler(const shared_ptr<Session> session) {
    const auto request = session->get_request();

    // Extract query parameters safely
    string db_name = request->get_query_parameter("db_name", "");
    string table_name = request->get_query_parameter("table_name", "");
    string id_str = request->get_query_parameter("id", "");

    // Validate input
    if (db_name.empty() || table_name.empty() || id_str.empty()) {
        session->close(BAD_REQUEST, "{\"error\": \"Missing query parameters\"}", {{"Content-Type", "application/json"}});
        return;
    }

    int id;
    try {
        id = stoi(id_str); // Convert ID to an integer
    } catch (const exception&) {
        session->close(BAD_REQUEST, "{\"error\": \"Invalid ID format\"}", {{"Content-Type", "application/json"}});
        return;
    }

    // Get the database instance
    Database& db = Database::getInstance(db_name);

    try {
        // Call the database delete_data function
        DeleteResult result = db.delete_data(table_name, id);

        // Handle the result of delete_data
        switch (result) {
            case DeleteResult::SUCCESS:
                session->close(OK, "{\"message\": \"Record deleted successfully\"}", {{"Content-Type", "application/json"}});
                break;

            case DeleteResult::ID_NOT_FOUND:
                session->close(NOT_FOUND, "{\"error\": \"Record not found\"}", {{"Content-Type", "application/json"}});
                break;

            case DeleteResult::SQL_EXECUTION_FAILED:
                session->close(INTERNAL_SERVER_ERROR, "{\"error\": \"Failed to execute delete operation\"}", {{"Content-Type", "application/json"}});
                break;

            default:
                session->close(INTERNAL_SERVER_ERROR, "{\"error\": \"Unknown error occurred\"}", {{"Content-Type", "application/json"}});
                break;
        }
    } catch (const exception& e) {
        // Handle exceptions during database operation
        session->close(INTERNAL_SERVER_ERROR, "{\"error\": \"Exception occurred: " + string(e.what()) + "\"}", {{"Content-Type", "application/json"}});
    }
}



void get_all_data_handler(const shared_ptr<Session> session) {
    const auto request = session->get_request();
    string db_name = request->get_query_parameter("db_name", "");
    string table_name = request->get_query_parameter("table_name", "");

    if (db_name.empty() || table_name.empty()) {
        session->close(BAD_REQUEST, "Missing query parameters", {{"Content-Type", "application/json"}});
        return;
    }

    Database& db = Database::getInstance(db_name);

    try {
        vector<map<string, DataType>> all_data = db.get_all_data(table_name);

        // Prepare JSON response
        json response_json = json::array();
        for (const auto& row : all_data) {
            json row_json;
            for (const auto& [key, value] : row) {
                if (holds_alternative<int>(value)) {
                    row_json[key] = get<int>(value);
                } else if (holds_alternative<double>(value)) {
                    row_json[key] = get<double>(value);
                } else if (holds_alternative<string>(value)) {
                    row_json[key] = get<string>(value);
                } else {
                    row_json[key] = nullptr; // Handle nulls
                }
            }
            response_json.push_back(row_json);
        }

        // Send JSON response
        session->close(OK, response_json.dump(), {{"Content-Type", "application/json"}});
    } catch (const exception& e) {
        session->close(INTERNAL_SERVER_ERROR, "{\"error\": \"Error retrieving data: " + string(e.what()) + "\"}", {{"Content-Type", "application/json"}});
    }
}



int main() {
    auto create_table_resource = make_shared<Resource>();
    create_table_resource->set_path("/create_table");
    create_table_resource->set_method_handler("POST", create_table_handler);

    auto add_column_resource=make_shared<Resource>();
    add_column_resource->set_path("/add_column");
    add_column_resource->set_method_handler("POST",add_column_handler);

    auto insert_data_resource = make_shared<Resource>();
    insert_data_resource->set_path("/insert_data");
    insert_data_resource->set_method_handler("POST", insert_data_handler);

    auto update_data_resource = make_shared<Resource>(); // Resource for updating data
    update_data_resource->set_path("/update_data");
    update_data_resource->set_method_handler("POST", update_data_handler);
    
  auto delete_data_resource = make_shared<Resource>();
  delete_data_resource->set_path("/delete_data");
  delete_data_resource->set_method_handler("DELETE", delete_data_handler);


    auto get_all_data_resource = make_shared<Resource>();
    get_all_data_resource->set_path("/get_all_data");
    get_all_data_resource->set_method_handler("GET", get_all_data_handler);

 
    auto settings = make_shared<Settings>();  // Corrected 'settings' to 'Settings'
    settings->set_port(8000);

    Service service;
    service.publish(create_table_resource);
    service.publish(add_column_resource);
    service.publish(insert_data_resource);
    service.publish(update_data_resource);
    service.publish(delete_data_resource);
     service.publish(get_all_data_resource);
    service.start(settings);
}

