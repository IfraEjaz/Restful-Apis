# Use an official Python runtime as a parent image
FROM python:3.8.19-alpine

# Set the working directory in the container
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Install any needed packages specified in requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Make port 7000 available to the world outside this container
EXPOSE 7000

# Run uvicorn server with the specified host and port
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "7000"]

