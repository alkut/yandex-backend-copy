FROM ubuntu:latest

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
        python3-pip \
        wget \
        tar

RUN git clone https://github.com/google/glog.git
WORKDIR glog

RUN \
    cmake -S . -B build -G "Unix Makefiles" \
    && cmake --build build \
    && cmake --build build --target test \
    && cmake --build build --target install

WORKDIR ./..

RUN git clone https://github.com/google/benchmark.git
WORKDIR benchmark

RUN \
    cmake -E make_directory "build" \
    && cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release .. \
    && cmake --build "build" --config Release \
    && cmake -E chdir "build" ctest --build-config Release \
    && cmake --build "build" --config Release --target install

WORKDIR ./..

RUN \
    wget ftp://ftp.unixodbc.org/pub/unixODBC/unixODBC-2.3.9.tar.gz \
    && tar xvzf unixODBC-2.3.9.tar.gz

WORKDIR unixODBC-2.3.9/

RUN \
    ./configure --prefix=/usr/local/unixODBC \
    && make \
    && make install \
    && export PATH=$PATH:/usr/local/unixODBC/bin \
    && pip install cpplint
