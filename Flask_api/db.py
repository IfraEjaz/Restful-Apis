import sqlite3
from sqlite3 import Error

lst = [
    {
        "id": 2,
        "name": "ABC",
        "age": 32,
        "department": "B"
    },
    {
        "id": 3,
        "name": "ABC",
        "age": 35,
        "department": "A"
    },
    {
        "id": 4,
        "name": "ABC",
        "age": 32,
        "department": "A"
    },
    {
        "id": 5,
        "name": "ABC",
        "age": 30,
        "department": "A"
    }
]

dicttosend = {
    "id": 1,
    "name": "ABC",
    "age": 35,
    "department": "A"
}

def create_connection(db_file):
    """ Create a database connection to an SQLite database """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        print(f"Connected to SQLite version {sqlite3.version}")
    except Error as e:
        print(f"Error: {e}")
    return conn

def create_table(conn):
    """ Create the 'projects' table if it doesn't exist """
    sql_create_projects_table = """
    CREATE TABLE IF NOT EXISTS projects (
        id integer PRIMARY KEY,
        name text NOT NULL,
        age integer,
        department text
    );
    """
    try:
        cur = conn.cursor()
        cur.execute(sql_create_projects_table)
        conn.commit()
    except Error as e:
        print(f"Error: {e}")

def insert_initial_data(conn, data):
    """ Insert initial data into the database if it doesn't exist """
    sql_insert_data = """
    INSERT OR IGNORE INTO projects (id, name, age, department) 
    VALUES (?, ?, ?, ?)
    """
    cur = conn.cursor()
    for entry in data:
        cur.execute(sql_insert_data, (entry["id"], entry["name"], entry["age"], entry["department"]))
    conn.commit()

def initialize_database(db_file, data):
    """ Initialize the database: create connection, create table, insert initial data """
    conn = create_connection(db_file)
    if conn is not None:
        create_table(conn)
        insert_initial_data(conn, data)
        conn.close()
    else:
        print("Error! Cannot create the database connection.")

if __name__ == '__main__':
    initialize_database("database.db", lst)

