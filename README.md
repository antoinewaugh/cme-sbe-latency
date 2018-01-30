# CME Depth Book Construction

This library is designed to be run on a server with access to CME multicast
traffic.

## Sample
```

```

## Build
```
docker build -t cme-sbe-latency .
```

## Usage

```
docker run cme-sbe-latency 0.0.0.0 <path_to_config.xml>
```

# Sample

```
docker run --net=host cme-sbe-latency  0.0.0.0 config.xml

 Security 23936, Recovering 0, LastSeqNum 2541935

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
# Todo
* incremental channel level gap - (currently only instrument level detection)
