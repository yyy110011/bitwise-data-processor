# bitwise-data-processor

## Setup source server
The server will listen to 127.0.0.1:5001.
```shell
cd tools/source_server
docker build -t source_server . 
docker run -p 5001:8888 source_server
```

## Setup result collect server
The server will listen to 127.0.0.1:5002.
```shell
cd tools/result_collect_server
docker build -t result_collect_server .
docker run -p 5002:8888 result_collect_server
```

## Build
```shell
apt-get update
apt-get install -y cmake g++ curl libgtest-dev libcurl4-gnutls-dev

mkdir build && cd build
cmake .. && make

# setup env
source ./setEnv

# run
./data_processor
```


