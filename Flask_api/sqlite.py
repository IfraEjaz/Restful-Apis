import sqlite3
from sqlite3 import Error

class Sqlite:
    def __init__(self, db_file, table_name):
        self.db_file = db_file
        self.table_name = table_name

    def connect(self):
        """Create a database connection"""
        conn = None
        try:
            conn = sqlite3.connect(self.db_file)
        except Error as e:
            print(f"Error: {e}")
        return conn

    def insert_data(self, data):
        """Insert data into the SQLite database"""
        sql_insert = f"INSERT INTO {self.table_name} (id, name, age, department) VALUES (?, ?, ?, ?)"
        conn = self.connect()
        if conn is not None:
            cur = conn.cursor()
            for entry in data:
                cur.execute(sql_insert, (entry["id"], entry["name"], entry["age"], entry["department"]))
            conn.commit()
            conn.close()

    def insert_data_with_id(self, id, data):
        """Insert data with a specific ID, checking for existence first"""
        if self.get_data(id) is not None:
            print(f"Resource with ID {id} already exists.")
            return
        data["id"] = id
        self.insert_data([data])

    def get_data(self, id=None):
        """Retrieve data from the SQLite database by ID, or all data if no ID"""
        conn = self.connect()
        if conn is not None:
            cur = conn.cursor()
            if id is None:
                cur.execute(f"SELECT * FROM {self.table_name}")
                rows = cur.fetchall()
            else:
                cur.execute(f"SELECT * FROM {self.table_name} WHERE id = ?", (id,))
                rows = cur.fetchone()
            conn.close()
            return rows

    def delete_data(self, id=None):
        """Delete data by ID or clear all records if no ID is provided"""
        conn = self.connect()
        if conn is not None:
            cur = conn.cursor()
            if id is None:
                cur.execute(f"DELETE FROM {self.table_name}")
                conn.commit()
                conn.close()
                return "All resources have been cleared."
            else:
                cur.execute(f"DELETE FROM {self.table_name} WHERE id = ?", (id,))
                if cur.rowcount == 0:
                    return "Resource doesn't exist."
                conn.commit()
                conn.close()
                return f"Resource with ID {id} removed successfully."

