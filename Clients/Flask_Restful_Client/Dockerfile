# Use an official Python runtime as a parent image
FROM python:3.9-slim

# Set the working directory in the container
WORKDIR /usr/src/app

# Copy the client script into the container
COPY restfulclient.py .

# Install the required packages
RUN pip install --no-cache-dir requests

# Run Flaskrestfulclient.py when the container launches
CMD ["python", "./restfulclient.py"]

