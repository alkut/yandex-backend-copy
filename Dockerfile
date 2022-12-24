FROM ubuntu

COPY . ./app

WORKDIR ./app
RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install cmake -y
RUN apt-get install libevent-dev -y
RUN apt-get install libboost-all-dev -y
RUN apt-get install pkg-config -y
RUN apt-get install libgtest-dev -y

RUN apt-get install git -y

RUN git clone https://github.com/google/glog.git
WORKDIR glog
RUN cmake -S . -B build -G "Unix Makefiles"
RUN cmake --build build
RUN cmake --build build --target test
RUN cmake --build build --target install
WORKDIR ./..

WORKDIR ./build

RUN cmake ./..
RUN make

