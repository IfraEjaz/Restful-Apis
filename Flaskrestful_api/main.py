from flask import Flask, request
from flask_restful import Resource, Api
from sqlite import Sqlite  # Import the Sqlite class
from db import initialize_database, lst  # Import the database initializer and initial data

app = Flask(__name__)  # Creating the flask app
api = Api(app)  # Creating an API object

# Initialize the database dynamically when the application starts
initialize_database("database.db", lst)

# Initialize the Sqlite instance with the 'database.db' and a table name
sql = Sqlite("database.db", "projects")


class Employee(Resource):
    """Flask Restful Child class for managing employees."""

    def get(self):
        """GET request method to host a resource."""
        try:
            rest_server_data = sql.get_data()  # Get all data
            return rest_server_data, 200  # Return data without jsonify
        except Exception as e:
            return {"error": str(e)}, 500

    def post(self):
        """POST request method to create a resource."""
        data = request.get_json(force=True)  # Get JSON data
        required_fields = ['id', 'name', 'age', 'department']
        if any(field not in data for field in required_fields):
            return {"error": "Missing required fields"}, 400
        if sql.get_data(data['id']):
            return {"error": "Resource with this ID already exists"}, 400
        sql.insert_data([data])  # Insert new data
        return {"message": "Employee Data has been Entered"}, 201

    def put(self):
        """PUT request method to update a resource."""
        data = request.get_json(force=True)  # Get JSON data
        if 'id' not in data:
            return {"error": "ID is required to update"}, 400
        if sql.get_data(data['id']):
            sql.insert_data_with_id(data['id'], data)  # Update existing data
            return {"message": "Employee Data has been Updated"}, 200
        return {"error": "Resource does not exist"}, 404

    def delete(self):
        """DELETE request method to remove all resources."""
        sql.delete_data()  # Clear all resources
        return {"message": "All resources have been cleared"}, 200


class EmployeeWithID(Resource):
    """Flask Restful Client Method for managing employees by ID."""

    def get(self, num):
        """GET request method to host a resource with specified ID."""
        data = sql.get_data(num)  # Get data by ID
        if data:
            return data, 200  # Return data without jsonify
        return {"error": "Resource doesn't exist"}, 404

    def post(self, num):
        """POST request method to create a resource with specified ID."""
        if sql.get_data(num):
            return {"error": "Resource already exists"}, 400
        input_json = request.get_json(force=True)
        required_fields = ['name', 'age', 'department']
        if any(field not in input_json for field in required_fields):
            return {"error": "Missing required fields"}, 400
        sql.insert_data_with_id(num, dict(input_json))
        return {"message": "Resource has been created"}, 201

    def put(self, num):
        """PUT request method to update a resource with specified ID."""
        input_json = request.get_json(force=True)
        if sql.get_data(num):
            sql.insert_data_with_id(num, dict(input_json))
            return {"message": "Resource updated successfully"}, 200
        return {"error": "Resource does not exist"}, 404

    def delete(self, num):
        """DELETE request method to remove a resource with specified ID."""
        if sql.get_data(num):
            sql.delete_data(num)
            return {"message": f"Employee with ID {num} has been successfully deleted"}, 200
        return {"error": "Resource doesn't exist"}, 404


class AllEmployees(Resource):
    """Resource to get all employees."""
    
    def get(self):
        """GET request method to retrieve all employees."""
        all_data = sql.get_all_employees()
        if all_data:
            return all_data, 200
        return {"message": "No employees found"}, 404


# Adding resources to the API
api.add_resource(Employee, '/')  # General Employee endpoint
api.add_resource(EmployeeWithID, '/event/<int:num>')  # Employee by ID
api.add_resource(AllEmployees, '/employees')  # Get all employees


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5002, debug=True)

