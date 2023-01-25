FROM ubuntu

RUN \
    apt-get update && \
    apt-get -y install \
        build-essential \
        cmake \
        libevent-dev \
        libboost-all-dev \
        pkg-config \
        libgtest-dev \
        libcurl4-gnutls-dev \
        git \
        valgrind \
        python3-pip

RUN git clone https://github.com/google/glog.git
WORKDIR glog

RUN \
    cmake -S . -B build -G "Unix Makefiles" \
    && cmake --build build \
    && cmake --build build --target test \
    && cmake --build build --target install

WORKDIR ./..

RUN pip install cpplint



