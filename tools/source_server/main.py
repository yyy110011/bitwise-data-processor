import os
from flask import Flask, Response
import random
import time

app = Flask(__name__)

RUN_TIMES = 10000
FREQUENCY = 200

FILE_NAME = "random_bits.txt"
SEND_FILE_SIZE = 128
data = []
counter = 0

def readFile(file_name):
    global data
    with open(file_name, 'rb') as f:
        tmp = f.read()
        amount = len(tmp) // SEND_FILE_SIZE
        for i in range(amount):
            data.append(tmp[i * SEND_FILE_SIZE: (i+1)*SEND_FILE_SIZE])
    print("Done")

@app.route("/")
def hello():
    return "Hello, World!"

def bitstring_to_bytes(s):
    return int(s, 2).to_bytes((len(s) + 7) // 8, byteorder='big')

def prev_identical_generate():
    for i in range(RUN_TIMES):
        data = ''.join(['0' for _ in range(256)]) + ''.join([str(random.randint(0, 1)) for _ in range(768)])
        data = bitstring_to_bytes(data)
        yield data
        time.sleep(1)

def generate_random():
    for i in range(RUN_TIMES):
        data = ''.join([str(random.randint(0, 1)) for _ in range(1024)])
        data = bitstring_to_bytes(data)
        yield data
        time.sleep(1)

def get_random_binary():
    global data
    for i in range(RUN_TIMES):
        rand_binary = data[i%10]
        yield rand_binary
        time.sleep(1)

@app.route("/random")
def random_binary():
    return Response(prev_identical_generate(), mimetype='text/plain')

if __name__ == '__main__':
    readFile(FILE_NAME)
    app.run(debug=True, host='0.0.0.0', port=8888)