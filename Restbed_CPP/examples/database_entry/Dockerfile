FROM ubuntu:20.04

# Set non-interactive mode for apt-get
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies including SQLite3 and nlohmann-json development libraries
RUN apt-get update && apt-get -y install \
    cmake \
    make \
    g++ \
    clang \
    libboost-all-dev \
    libssl-dev \
    libasio-dev \
    libsqlite3-dev \
    nlohmann-json3-dev

# Copy the entire project into the container
COPY . /app

# Build Restbed library
RUN mkdir -p /app/restbed/build
WORKDIR /app/restbed/build 
RUN cmake -DBUILD_SSL=NO -DBUILD_TESTS=NO ..
RUN make
RUN make install

# Set the LD_LIBRARY_PATH so the system can find librestbed.so.4 at runtime
ENV LD_LIBRARY_PATH=/usr/local/lib

# Build the Restful application
WORKDIR /app
# Build the Restful application with C++17 standard
RUN clang++ -std=c++17 -o restful-app restful-app.cpp database.cpp -lrestbed -lboost_program_options -lsqlite3


# Expose the port to the container
EXPOSE 8000

# Command to run the application
CMD ["./restful-app", "--host", "0.0.0.0", "--port", "8000"]

