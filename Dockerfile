# Base image
FROM ubuntu:20.04

# Set environment variables
ENV SOURCE_HOSTNAME "http://127.0.0.1:5001/random"
ENV SOURCE_SIZE "128"
ENV IDENTICAL_SIZE "32"

ENV CHANNEL_ID "1024"
ENV OUTPUT_HOSTNAME "http://127.0.0.1:5002/ingest"
ENV OUTPUT_AMOUNT "3"

# Set the working directory to /app
WORKDIR /app

# Install necessary packages for building C++ projects
RUN apt-get update && \
    apt-get install -y cmake g++ curl libgtest-dev libcurl4-gnutls-dev

# Copy the source code into the container
COPY . /app

# Build the project using cmake
RUN cmake . && make

# Set the default command to run the project
# CMD ["./data_processor"]
