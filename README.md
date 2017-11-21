# CME Depth Book Construction

This application is designed to be run on a server with access to CME multicast
traffic.

## Build
```
docker build -t cme-sbe-latency .
```

## Usage

```
docker run cme-sbe-latency 0.0.0.0 \
	<multicast_ip_incrementalA> \
	<multicast_port_incrementalA> \
	<multicast_ip_incrementalB> \
	<multicast_port_incrementalB> \
	<multicast_ip_snapshotA> \
	<multicast_port_snapshotA> \
	<multicast_ip_shapshotB> \
	<multicast_port_snapshotB> 

# Sample

```
docker run --net=host cme-sbe-latency  0.0.0.0 224.0.31.1 14310 224.0.32.1 15310 224.0.31.22 14310 224.0.32.22 15310 

Security: 23936, Recovering: 0, LastSeqNum: 2541935
BidV		Price		AskV
--------------------------------------
		258475		364
		258450		355
		258425		330
		258400		474
		258375		315
		258350		275
		258325		209
		258300		176
		258275		171
		258250		44
--------------------------------------
136		258225
186		258200
201		258175
186		258150
351		258125
256		258100
260		258075
292		258050
407		258025
428		258000


```

