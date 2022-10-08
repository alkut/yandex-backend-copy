FROM ubuntu

COPY . ./app

WORKDIR ./app
RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install cmake -y
RUN apt-get install libevent-dev -y
RUN apt-get install libboost-all-dev -y
RUN apt-get install pkg-config -y
RUN apt-get install libgoogle-glog-dev -y

WORKDIR ./build

WORKDIR ../
RUN cmake .
RUN make
RUN ./yad

