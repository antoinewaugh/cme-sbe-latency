# CME Depth Book Construction

This application is designed to be run on a server with access to CME multicast
traffic.

## Build
```
docker build -t cme-sbe-latency .
```

## Usage

docker run cme-sbe-latency 0.0.0.0 \
	<multicast_ip_incrementalA> \
	<multicast_port_incrementalA> \
	<multicast_ip_incrementalB> \
	<multicast_port_incrementalB> \
	<multicast_ip_snapshotA> \
	<multicast_port_snapshotA> \
	<multicast_ip_shapshotB> \
	<multicast_port_snapshotB> 

```
docker run --net=host cme-sbe-latency  0.0.0.0 224.0.31.1 14310 224.0.32.1 15310 224.0.31.22 14310 224.0.32.22 15310 

```

# Todo:

* Orderbook Tests
* Recover Mode
