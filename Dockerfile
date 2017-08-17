
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
  libboost-all-dev \
  software-properties-common

# Java
RUN	apt-get -y purge openjdk-\* \
	&& add-apt-repository ppa:webupd8team/java \
	&& apt-get -y update \
	&& echo "oracle-java8-installer shared/accepted-oracle-license-v1-1 select true" | debconf-set-selections \
	&& apt-get -y install oracle-java8-installer 

ENV JAVA_HOME /usr/lib/jvm/java-8-oracle

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
