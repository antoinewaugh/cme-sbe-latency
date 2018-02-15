# CME MDP Adapter

This project and codebase was inspired by https://github.com/epam/java-cme-mdp3-handler.

# License

https://www.gnu.org/licenses/lgpl-3.0.en.html

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

## Installation

## install boost
```
wget https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz
tar -xvf boost_1_63_0.tar.gz
cd boost_1_63_0
sudo ./boostrap
sudo ./b2 install
```

## install sbe

```
su
git clone https://github.com/real-logic/agrona \
	&& cd agrona \
	&& ./gradlew \
	&& cd .. \
	&& git clone https://github.com/real-logic/simple-binary-encoding.git sbe \
	&& cd sbe \
	&& ./gradlew \
	&& ./cppbuild/cppbuild \
	&& wget ftp://ftp.cmegroup.com/SBEFix/Production/Templates/templates_FixBinary.xml \
	&& mkdir include \
	&& java -Dsbe.keyword.append.token=_ -Dsbe.target.language=Cpp -Dsbe.target.namespace=sbe -Dsbe.output.dir=include -Dsbe.errorLog=yes -jar sbe-all/build/libs/sbe-all-1.7.6-SNAPSHOT.jar templates_FixBinary.xml \
	&& mv include/sbe /usr/local/include \
	&& mv sbe-tool/src/main/cpp/sbe /usr/local/include/sbe \
	&& mv cppbuild/Release/lib/libsbe.a /usr/local/lib \
	&& cd .. \
	&& rm -rf /sbe

```

# Todo
* incremental channel level gap - (currently only instrument level detection)
