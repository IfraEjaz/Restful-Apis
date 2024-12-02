import requests

# Use a single slash at the end of BASE_URL
BASE_URL = "http://fastapi:7000/"  # Assuming FastAPI is running locally on port 7000

# Test the root endpoint to get all employees
def get_all_employees():
    response = requests.get(f"{BASE_URL}/")
    if response.status_code == 200:
        print("Employees:", response.json())
    else:
        print("Status:", response.status_code, response.text)

# Get an employee by ID
def get_employee_by_id(emp_id):
    response = requests.get(f"{BASE_URL}/getDatabyID/{emp_id}")
    if response.status_code == 200:
        print(f"Employee {emp_id}:", response.json())
    else:
        print("Status:", response.status_code, response.text)

# Get an employee by name
def get_employee_by_name(name):
    response = requests.get(f"{BASE_URL}/getDatabyName/", params={'name': name})
    if response.status_code == 200:
        print(f"Employee with name {name}:", response.json())
    else:
        print("Status:", response.status_code, response.text)

# Create a new employee
def create_employee(emp_id, data):
    response = requests.post(f"{BASE_URL}/postData/{emp_id}", json=data)
    if response.status_code == 201:
        print("Employee created successfully")
    else:
        print("Status:", response.status_code, response.text)

# Update an existing employee
def update_employee(emp_id, data):
    response = requests.put(f"{BASE_URL}/updateData/{emp_id}", json=data)
    if response.status_code == 200:
        print("Employee updated successfully")
    else:
        print("Status:", response.status_code, response.text)

# Delete an employee
def delete_employee(emp_id):
    response = requests.delete(f"{BASE_URL}/deleteEmployee/{emp_id}")
    if response.status_code == 200:
        print(f"Employee {emp_id} deleted successfully")
    else:
        print("Status:", response.status_code, response.text)

if __name__ == "__main__":
    # Test getting all employees
    print("Get all employees:")
    get_all_employees()

    # Test creating a new employee
    print("\nCreate a new employee:")
    new_employee = {
        "name": "Faiza",
        "age": 32,
        "department": "B"
    }
    create_employee(5, new_employee)

    # Test creating another new employee
    print("\nCreate another new employee:")
    another_employee = {
        "name": "Sara",
        "age": 30,
        "department": "B"
    }
    create_employee(6, another_employee)

    # Test getting employee by ID
    print("\nGet employee by ID:")
    get_employee_by_id(2)

    # Test getting employee by name
    print("\nGet employee by name:")
    get_employee_by_name("Faiza")

    # Test updating an employee
    print("\nUpdate an employee:")
    updated_employee = {
        "name": "Alina",
        "age": 35,
        "department": "A"
    }
    update_employee(2, updated_employee)

    # Test deleting an employee
    print("\nDelete an employee:")
    delete_employee(2)

    # Test getting all employees again to see changes
    print("\nGet all employees after changes:")
    get_all_employees()

