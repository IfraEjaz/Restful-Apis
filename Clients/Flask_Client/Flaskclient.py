import requests
import json

BASE_URL = "http://flaskapi:5000/"  # The base URL of your Flask API

def insert_resource(id, data):
    """Inserts a resource with the given ID and data."""
    url = f"{BASE_URL}/resource/{id}"
    response = requests.post(url, json=data)
    print(f"POST {url}: {response.status_code} - {response.text}")

def get_resource(id):
    """Retrieves a resource by its ID."""
    url = f"{BASE_URL}/resource/{id}"
    response = requests.get(url)
    print(f"GET {url}: {response.status_code} - {response.text}")
    
def get_all_resources():
    """Retrieves all resources."""
    url = f"{BASE_URL}/resources"
    response = requests.get(url)
    print(f"GET {url}: {response.status_code} - {response.text}")
    
def update_resource(id, data):
    """Updates a resource with the given ID."""
    url = f"{BASE_URL}/resource/{id}"
    response = requests.put(url, json=data)
    print(f"PUT {url}: {response.status_code} - {response.text}")

def delete_resource(id):
    """Deletes a resource by its ID."""
    url = f"{BASE_URL}/resource/{id}"
    response = requests.delete(url)
    print(f"DELETE {url}: {response.status_code} - {response.text}")

def delete_all_resources():
    """Deletes all resources."""
    url = f"{BASE_URL}/resources"
    response = requests.delete(url)
    print(f"DELETE {url}: {response.status_code} - {response.text}")


if __name__ == "__main__":
    # Test data for inserting/updating resources
    test_data = {
        "id": 9,
        "name": "maria",
        "age": 32,
        "department": "A"
    }
    
    test_data_update = {
        "id": 3,
        "name": "john",
        "age": 32,
        "department": "A"
    }

    # Insert a new resource
    print("Inserting resource with ID 3:")
    insert_resource(3, test_data)
    
    # Get the inserted resource
    print("\nRetrieving resource with ID 3:")
    get_resource(3)
    
    # Get all resources
    print("\nRetrieving all resources:")
    get_all_resources()

    # Update the inserted resource
    print("\nUpdating resource with ID 3:")
    update_resource(3, test_data_update)
    
    # Get the updated resource
    print("\nRetrieving updated resource with ID 3:")
    get_resource(3)

    # Delete the resource
    print("\nDeleting resource with ID 3:")
    delete_resource(3)

    # Verify that the resource has been deleted
    print("\nVerifying deletion of resource with ID 3:")
    get_resource(3)
    
    # Delete all resources
    #print("\nDeleting all resources:")
    #delete_all_resources()

    # Verify that all resources have been deleted
    print("\nVerifying deletion of all resources:")
    get_all_resources()

