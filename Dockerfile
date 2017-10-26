
FROM ubuntu:16.04

MAINTAINER Antoine Waugh

# Developer dependencies
RUN apt-get update && apt-get -y install \
  apt-utils \
  g++ \
  wget \
  make \
  cmake \
  git \
  libboost-all-dev 

WORKDIR /opt/jdk
RUN wget --no-check-certificate -c --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u151-b12/e758a0de34e24606bca991d704f6dcbf/jdk-8u151-linux-x64.tar.gz \
       &&   tar -zxf jdk-8u151-linux-x64.tar.gz -C /opt/jdk \
       &&   update-alternatives --install /usr/bin/java java /opt/jdk/jdk1.8.0_151/bin/java 100 \
       &&   update-alternatives --install /usr/bin/javac javac /opt/jdk/jdk1.8.0_151/bin/javac 100

ENV JAVA_HOME /opt/jdk/jdk1.8.0_151

# Install SBE Lib & Headers

WORKDIR /

RUN git clone https://github.com/real-logic/agrona \
	&& cd agrona \
	&& ./gradlew \
	&& cd .. \
	&& git clone https://github.com/real-logic/simple-binary-encoding.git sbe \
	&& cd sbe \
	&& ./gradlew \
	&& ./cppbuild/cppbuild \
	&& wget ftp://ftp.cmegroup.com/SBEFix/Production/Templates/templates_FixBinary.xml \
	&& mkdir include \
	&& java -Dsbe.keyword.append.token=_ -Dsbe.target.language=Cpp -Dsbe.target.namespace=sbe -Dsbe.output.dir=include -Dsbe.errorLog=yes -jar sbe-all/build/libs/sbe-all-1.7.1-SNAPSHOT.jar templates_FixBinary.xml \
	&& mv include/sbe /usr/local/include \
	&& mv sbe-tool/src/main/cpp/sbe /usr/local/include/sbe \
	&& mv cppbuild/Release/lib/libsbe.a /usr/local/lib \
	&& cd .. \
	&& rm -rf /sbe

COPY . /src

WORKDIR /src
RUN mkdir build \
	&& cd build \
	&& cmake .. && make \
  && mv cme_sbe_latency /usr/local/bin


ENTRYPOINT ["/usr/local/bin/cme_sbe_latency"]
CMD ["0.0.0.0", "224.0.32.1", "15310"]
