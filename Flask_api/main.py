from flask import Flask, request, jsonify
from sqlite import Sqlite  # Import the Sqlite class
from db import initialize_database, lst  # Import the database initializer and initial data

app = Flask(__name__)

# Initialize the database dynamically when the application starts
initialize_database("database.db", lst)

# Initialize the Sqlite instance with the 'database.db' and a table name
sql = Sqlite(r"database.db", "projects")

@app.route('/resources', methods=['GET', 'POST', 'PUT', 'DELETE'])
def events():
    if request.method == 'GET':
        rest_server_data = sql.get_data()  # Get all data
        return jsonify(rest_server_data), 200

    if request.method == 'POST':
        input_json = request.get_json(force=True)
        data_req = dict(input_json)
        if sql.get_data(data_req['id']):
            return "Resource with this ID already exists", 404
        else:
            sql.insert_data([data_req])  # Insert new data
            return "Resource has been created", 201

    if request.method == 'PUT':
        input_json = request.get_json(force=True)
        data_req = dict(input_json)
        if sql.get_data(data_req['id']):
            sql.insert_data_with_id(data_req['id'], data_req)
            return "Resource updated successfully", 200
        else:
            return "Resource does not exist", 404

    if request.method == 'DELETE':
        sql.delete_data()  # Clear all resources
        return "All resources have been cleared", 200

@app.route('/resource/<int:id>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def event(id):
    if request.method == 'GET':
        data = sql.get_data(id)  # Get data by ID
        if data:
            return jsonify(data), 200
        else:
            return "Resource doesn't exist", 404

    if request.method == 'POST':
        input_json = request.get_json(force=True)
        if sql.get_data(id):
            return "Resource already exists", 404
        else:
            sql.insert_data_with_id(id, dict(input_json))
            return "Resource has been created", 201

    if request.method == 'PUT':
        input_json = request.get_json(force=True)
        if sql.get_data(id):
            sql.insert_data_with_id(id, dict(input_json))
            return "Resource updated successfully", 200
        else:
            return "Resource does not exist", 404

    if request.method == 'DELETE':
        result = sql.delete_data(id)
        return result, 200

if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)

