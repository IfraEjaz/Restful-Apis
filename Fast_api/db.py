import sqlite3
from sqlite3 import Error

class Sqlite:
    def __init__(self, db_file):
        self.db_file = db_file
        self.create_table()  # Create the employees table when initializing the class
        self.insert_initial_data()  # Insert initial data

    def create_connection(self):
        """Create a database connection to a SQLite database."""
        try:
            conn = sqlite3.connect(self.db_file)
            return conn
        except Error as e:
            print(e)
        return None

    def create_table(self):
        """Create the employees table if it doesn't exist."""
        conn = self.create_connection()
        if conn:
            try:
                sql = '''CREATE TABLE IF NOT EXISTS employees (
                            id INTEGER PRIMARY KEY,
                            name TEXT NOT NULL,
                            age INTEGER NOT NULL,
                            department TEXT NOT NULL
                        );'''
                cur = conn.cursor()
                cur.execute(sql)
                conn.commit()
            except Error as e:
                print(f"Error creating table: {e}")
            finally:
                conn.close()

    def insert_initial_data(self):
        """Insert initial data into the SQLite database."""
        initial_data = [
            (1, 'John Doe', 30, 'HR'),
            (2, 'Jane Smith', 25, 'IT'),
            (3, 'Mike Johnson', 40, 'Finance'),
            # Add more initial data as needed
        ]
        for id, name, age, department in initial_data:
            self.insert_data(id, {'name': name, 'age': age, 'department': department})

    def insert_data(self, id, data):
        """Insert data into the SQLite database."""
        conn = self.create_connection()
        if conn:
            try:
                sql = '''INSERT INTO employees(id, name, age, department)
                          VALUES(?, ?, ?, ?)'''
                cur = conn.cursor()
                cur.execute(sql, (id, data['name'], data['age'], data['department']))
                conn.commit()
            except Error as e:
                print(f"Error inserting data: {e}")
            finally:
                conn.close()

    def update_data(self, id, data):
        """Update data in the SQLite database."""
        conn = self.create_connection()
        if conn:
            try:
                sql = '''UPDATE employees
                          SET name = ?,
                              age = ?,
                              department = ?
                          WHERE id = ?'''
                cur = conn.cursor()
                cur.execute(sql, (data['name'], data['age'], data['department'], id))
                conn.commit()
            except Error as e:
                print(f"Error updating data: {e}")
            finally:
                conn.close()

    def get_data(self, id=None):
        """Get data from the SQLite database."""
        conn = self.create_connection()
        if conn:
            try:
                cur = conn.cursor()
                if id is None:
                    cur.execute("SELECT * FROM employees")
                else:
                    cur.execute("SELECT * FROM employees WHERE id=?", (id,))
                rows = cur.fetchall()
                return rows
            except Error as e:
                print(f"Error fetching data: {e}")
            finally:
                conn.close()

    def delete_data(self, id):
        """Delete data from the SQLite database."""
        conn = self.create_connection()
        if conn:
            try:
                sql = 'DELETE FROM employees WHERE id=?'
                cur = conn.cursor()
                cur.execute(sql, (id,))
                conn.commit()
            except Error as e:
                print(f"Error deleting data: {e}")
            finally:
                conn.close()

