import requests

class RESTclient:
    def __init__(self, serverurl, db_name):
        # Define the server URL and database name once
        self.url = serverurl
        self.db_name = db_name

    def create_table(self, table_name):
        """Create a table in the database."""
        params = {
            'db_name': self.db_name,
            'table_name': table_name
        }
        try:
            response = requests.post(f"{self.url}/create_table", params=params)
            response.raise_for_status()  # This will raise an exception for HTTP error responses
            print(f"Table created successfully: {response.text}")  # Success message with response text
        except requests.RequestException as e:
            print(f"Error creating table: {e}")  # Detailed error message

    def add_column(self, table_name, column_name, data_type):
        """Add a column to a table."""
        params = {
            'db_name': self.db_name,
            'table_name': table_name,
            'column_name': column_name,
            'data_type': data_type
        }
        try:
            response = requests.post(f"{self.url}/add_column", params=params)
            response.raise_for_status()
            print(f"Column '{column_name}' added successfully: {response.text}")
        except requests.HTTPError as e:
            if response.status_code == 400:
               print(f"Error adding column '{column_name}': Invalid column name.")
            elif response.status_code == 409:
               print(f"Error adding column '{column_name}': Column already exists.")
            elif response.status_code == 500:
               print(f"Error adding column '{column_name}': {response.json()['error']}")
            else:
               print(f"Unexpected error: {e}")
    def insert_data(self, table_name, data):
        """Insert data into a table."""
        url = f"{self.url}/insert_data?db_name={self.db_name}&table_name={table_name}"
        headers = {
            'Content-Type': 'application/x-www-form-urlencoded'
        }
        body = '&'.join([f"{key}={value}" for key, value in data.items()])
        try:
          response = requests.post(url, headers=headers, data=body)
          response.raise_for_status()
          print(f"Data inserted successfully: {response.text}")
        except requests.RequestException as e:
          if response is not None:
             print(f"Error inserting data: {response.status_code} - {response.text}")
          else:
             print(f"Error inserting data: {e}")


    def update_data(self, table_name, id, data):
      """Update data in a table."""
      url = f"{self.url}/update_data?db_name={self.db_name}&table_name={table_name}&id={id}"
      headers = {
        'Content-Type': 'application/x-www-form-urlencoded'
      }
      body = '&'.join([f"{key}={value}" for key, value in data.items()])
      try:
          response = requests.post(url, headers=headers, data=body)
          response.raise_for_status()  # Raises an exception for 4xx/5xx status codes
          print(f"Data updated successfully: {response.text}")
      except requests.RequestException as e:
          try:
              error_details = response.json()  # Parse JSON error response
              print(f"Error updating data ({response.status_code}): {error_details.get('error', 'Unknown error')}")
          except Exception:
              print(f"Error updating data: {e}")


    def delete_data(self, table_name, id):
       """Delete data from a table."""
       url = f"{self.url}/delete_data"
       params = {
         'db_name': self.db_name,
         'table_name': table_name,
         'id': id
     }
       try:
         # Send DELETE request with query parameters
           response = requests.delete(url, params=params)
           response.raise_for_status()  # Raises an exception for HTTP error responses

         # Handle successful response
           if response.status_code == 200:
              print(f"Data deleted successfully: {response.json()['message']}")
           elif response.status_code == 404:
              print(f"Error: {response.json()['error']}")
           else:
              print(f"Unexpected response: {response.status_code} - {response.text}")
       except requests.HTTPError as e:
           # Handle known HTTP error scenarios
           if response.status_code == 400:
              print("Error: Missing or invalid query parameters.")
           elif response.status_code == 404:
              print("Error: Record not found.")
           elif response.status_code == 500:
              print(f"Server error: {response.json().get('error', 'Unknown server error')}")
           else:
              print(f"Unexpected HTTP error: {e}")
       except requests.RequestException as e:
            # Handle other request-related errors
           print(f"Error deleting data: {e}")


    def get_all_data(self, table_name):
        """Fetch all data from a table."""
        url = f"{self.url}/get_all_data?db_name={self.db_name}&table_name={table_name}"
        try:
            response = requests.get(url)
            response.raise_for_status()
            print(f"All data retrieved successfully: {response.json()}")  # Displaying data in JSON format
        except requests.RequestException as e:
            print(f"Error fetching data: {e}")


# Main logic block
if __name__ == "__main__":
    server_url = "http://localhost:8000"
    db_name = "my_database.db"

    # Create the client instance
    client = RESTclient(server_url, db_name)

    # Example: Create a valid table
    table_name = "My_table"
    print("Test: Creating valid table")
    client.create_table(table_name)

    # Example: Add valid columns
    print("\nTest: Adding columns to the table")
    client.add_column(table_name, "Name", "STRING")
    client.add_column(table_name, "Age", "INTEGER")
    client.add_column(table_name, "height", "REAL")

    # Test 1: Insert data with an empty payload (should return an empty data error)
    print("\nTest 1: Insert data with an empty payload")
    empty_data = {}
    client.insert_data(table_name, empty_data)

    # Test 2: Insert data into a non-existent table (should return a table not found error)
    print("\nTest 2: Insert data into a non-existent table")
    non_existent_table = "Non_existent_table"
    test_data = {}
    client.insert_data(non_existent_table, test_data)

    # Test 3: Insert data with a non-existent column (should return a column not found error)
    print("\nTest 3: Insert data with a non-existent column")
    data_with_wrong_column = {"Name": "John", "Age": 30, "Weight": 70.5}  # "Weight" does not exist
    client.insert_data(table_name, data_with_wrong_column)

    # Test 4: Insert data with an invalid data type (should return an invalid data error)
    print("\nTest 4: Insert data with an invalid data type")
    data_with_invalid_type = {"Name": "Karl", "Age": 30, "Height": "Tall"}  # "Height" should be a REAL value
    client.insert_data(table_name, data_with_invalid_type)

    # Test 5: Simulate SQL query failure (if applicable)
    print("\nTest 5: Simulated SQL query error")
    data_for_sql_error = {"Name": "Sim", "Age": 29, "Height": 6.1}
    client.insert_data(table_name, data_for_sql_error)

    # Test 6: Update data
    print("\nTest 6: Update data")
    update_data = {"Name":"xyz"}
    client.update_data(table_name, 222, update_data)  # Assuming `id=22` exists

    # Test 7: Delete data
    print("\nTest 7: Delete data")
    client.delete_data(table_name, 22)  # Delete record with id=22

    # Test 8: Get all data
    print("\nTest 8: Get all data")
    client.get_all_data(table_name)  # Fetch all records from the table

