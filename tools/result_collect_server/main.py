import os
from flask import Flask, Response, request

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello, World!"

@app.route("/ingest", methods=['POST'])
def ingest():
    data = request.get_data().decode().split(',')
    channel_id = data[0]
    timestamps = data[1::2]
    scores =  data[2::2]
    print(data)
    print(channel_id)
    print(timestamps)
    print(scores)
    return ""

if __name__ == '__main__':
    # app.run(debug=True, host='0.0.0.0', port=8888)
    app.run(debug=True, host='0.0.0.0', port=5002)