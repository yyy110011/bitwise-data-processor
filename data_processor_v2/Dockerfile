FROM python:3.8

WORKDIR /app

# Copy the application files
COPY requirements.txt .
COPY data_processor.py .

# Install the required packages
RUN pip install -r requirements.txt

# Start the data processor
CMD [ "python", "data_processor.py" ]
