# Base image
FROM python:3.9-slim-buster

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Install the required packages
RUN python3 -m pip install --no-cache-dir -r requirements.txt



# Start the Flask server
CMD ["python3", "main.py"]
