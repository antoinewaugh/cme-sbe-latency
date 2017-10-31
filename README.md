# CME Depth Latency Measurement

This application is designed to be run on a server with access to CME multicast
traffic.

## Build
```
docker build -t cme-sbe-latency .
```

## Usage

docker run cme-sbe-latency 0.0.0.0 <multicast_ip> <multicast_port>

```
docker run --net=host cme-sbe-latency 0.0.0.0 224.0.32.1 15310
```

# Todo:

* Orderbook Tests
* Recover Mode
