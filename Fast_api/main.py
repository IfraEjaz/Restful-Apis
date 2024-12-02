import uvicorn
from fastapi import FastAPI, Path, HTTPException
from typing import Optional
from pydantic import BaseModel
from db import Sqlite  # Import the Sqlite class

app = FastAPI()

# Initialize the Sqlite instance
sql = Sqlite("employees.db")

# Define Pydantic models for employee data validation
class Employee(BaseModel):
    name: str
    age: int
    department: str

class UpdateEmployee(BaseModel):
    name: Optional[str]
    age: Optional[int]
    department: Optional[str]

@app.get("/")
async def root():
    """Simple GET request that retrieves all data from the database."""
    rows = sql.get_data()
    return rows

@app.get("/getDatabyID/{id}")
async def get_data_by_id(id: int):
    """Get data by specific ID."""
    row = sql.get_data(id)
    if not row:
        raise HTTPException(status_code=404, detail="Resource doesn't exist")
    return row

@app.get("/getDatabyName/")
async def get_data_by_name(name: str):
    """Get data by name."""
    rows = sql.get_data()
    for row in rows:
        if row[1] == name:  # Assuming name is at index 1 in the data tuple
            return row
    raise HTTPException(status_code=404, detail="Resource doesn't exist")

@app.post("/postData/{id}")
async def post_custom_data(id: int, data: Employee):
    """Insert new data into the database."""
    rows = sql.get_data(id)
    if rows:
        raise HTTPException(status_code=409, detail="Resource Already Exists")
    sql.insert_data(id, data.dict())
    return {"detail": "Resource created successfully"}, 201  # Return 201 status code

@app.put("/updateData/{id}")
async def update_custom_data(id: int, data: UpdateEmployee):
    """Update existing data."""
    rows = sql.get_data(id)
    if not rows:
        raise HTTPException(status_code=404, detail="Resource doesn't exist")
    update_data = {k: v for k, v in data.dict().items() if v is not None}
    sql.update_data(id, update_data)
    return {"detail": "Resource updated successfully"}

@app.delete("/deleteEmployee/{id}")
async def delete_employee(id: int):
    """Delete data by ID."""
    rows = sql.get_data(id)
    if not rows:
        raise HTTPException(status_code=404, detail="Resource doesn't exist")
    sql.delete_data(id)
    return {"detail": "Resource removed successfully"}

if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=7000, reload=False, log_level="debug", workers=1)

