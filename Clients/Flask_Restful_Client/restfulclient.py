import requests

BASE_URL = 'http://flaskrestfulapi:5002'

# GET request to fetch employee data by ID
def get_employee(employee_id):
    response = requests.get(f"{BASE_URL}/event/{employee_id}")
    if response.status_code == 200:
        print("Employee found:", response.json())
    else:
        print("Error:", response.status_code, response.json())

# POST request to create an employee
def create_employee(employee_id, name, age, department):
    data = {
        'id': employee_id,
        'name': name,
        'age': age,
        'department': department
    }
    response = requests.post(f"{BASE_URL}/", json=data)
    if response.status_code == 201:
        print("Employee created successfully:", response.json())
    else:
        print("Error creating employee:", response.status_code, response.json())

# PUT request to update an employee
def update_employee(employee_id, name, age, department):
    data = {
        'id': employee_id,
        'name': name,
        'age': age,
        'department': department
    }
    response = requests.put(f"{BASE_URL}/", json=data)
    if response.status_code == 200:
        print("Employee updated successfully:", response.json())
    else:
        print("Error updating employee:", response.status_code, response.json())

# DELETE request to remove an employee by ID
def delete_employee(employee_id):
    response = requests.delete(f"{BASE_URL}/event/{employee_id}")
    if response.status_code == 200:
        print("Employee deleted successfully:", response.json())
    else:
        print("Error deleting employee:", response.status_code, response.json())

# GET request to fetch all employees
def get_all_employees():
    response = requests.get(f"{BASE_URL}/employees")
    if response.status_code == 200:
        print("All employees:", response.json())
    else:
        print("Error fetching employees:", response.status_code, response.json())

# Example usage:
if __name__ == '__main__':
    create_employee(8, 'John Doe', 30, 'HR')
    get_employee(8)
    update_employee(8, 'John Doe', 31, 'HR')
    get_employee(8)
    delete_employee(8)

    # Get all employees
    get_all_employees()

