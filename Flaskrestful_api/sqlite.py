import sqlite3
from sqlite3 import Error

class Sqlite:
    def __init__(self, db_file, table_name):
        self.db_file = db_file
        self.table_name = table_name

    def connect(self):
        """Create a database connection."""
        conn = None
        try:
            conn = sqlite3.connect(self.db_file)
        except Error as e:
            print(f"Error: {e}")
        return conn

    def insert_data(self, data):
        """Insert data into the SQLite database."""
        sql_insert = f"INSERT INTO {self.table_name} (id, name, age, department) VALUES (?, ?, ?, ?)"
        conn = self.connect()
        if conn is not None:
            cur = conn.cursor()
            for entry in data:
                cur.execute(sql_insert, (entry["id"], entry["name"], entry["age"], entry["department"]))
            conn.commit()
            conn.close()

    def insert_data_with_id(self, id, data):
        """Insert data with a specific ID, checking if the record exists."""
        conn = self.connect()
        sql_insert = f"INSERT OR REPLACE INTO {self.table_name} (id, name, age, department) VALUES (?, ?, ?, ?)"
        cur = conn.cursor()
        cur.execute(sql_insert, (id, data["name"], data["age"], data["department"]))
        conn.commit()
        conn.close()

    def get_data(self, id=None):
        """Get data from the SQLite database."""
        conn = self.connect()
        cur = conn.cursor()
        if id is not None:
            cur.execute(f"SELECT * FROM {self.table_name} WHERE id=?", (id,))
        else:
            cur.execute(f"SELECT * FROM {self.table_name}")
        rows = cur.fetchall()
        conn.close()
        return rows

    def get_all_employees(self):
        """Get all employee data from the SQLite database."""
        conn = self.connect()
        cur = conn.cursor()
        cur.execute(f"SELECT * FROM {self.table_name}")
        rows = cur.fetchall()
        conn.close()
        return rows

    def delete_data(self, id=None):
        """Delete data from the SQLite database."""
        conn = self.connect()
        cur = conn.cursor()
        if id:
            cur.execute(f"DELETE FROM {self.table_name} WHERE id=?", (id,))
        else:
            cur.execute(f"DELETE FROM {self.table_name}")
        conn.commit()
        conn.close()

