# CME MDP Adapter

This project and codebase was inspired by https://github.com/epam/java-cme-mdp3-handler.

# License

https://www.gnu.org/licenses/lgpl-3.0.en.html

This library is designed to be run on a server with access to CME multicast
traffic.

## Usage

```
mkdir build && cd build
cmake -DAPAMA_HOME=/path/to/apama .. && make
cd ..
wget ftp://ftp.cmegroup.com/SBEFix/Production/Configuration/config.xml
correlator --config cme-connectivity.yaml --config correlator.yaml
```

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
* handle disconnects/timeouts from network interface
* handle heartbeats
* complete interface
* elegant killing of thread(s)
* merge implied book
* prevent callbacks when impl/main book unchanged
* top of book?
