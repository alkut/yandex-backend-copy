FROM ubuntu:latest

ARG TZ=Etc/UTC

ARG DEBIAN_FRONTEND=noninteractive

RUN \
    apt-get update && \
    apt-get -y install \
        build-essential \
        pkg-config \
        libcurl4-gnutls-dev \
        valgrind \
        python3-pip \
        wget \
        tar \
        zip \
        git \
	    openssl \
	    libssl-dev \
        postgresql \
        postgresql-contrib \
        libpq-dev

RUN \
   wget https://github.com/Kitware/CMake/releases/download/v3.25.2/cmake-3.25.2.tar.gz -O /cmake-3.25.2.tar.gz && \
    tar zxvf /cmake-3.25.2.tar.gz -C /
WORKDIR /cmake-3.25.2
RUN \
   /cmake-3.25.2/bootstrap && \
    make -j4 && \
    make install

WORKDIR /
RUN \
   apt-get -y install \
    ninja-build

RUN \
    wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz -O /boost_1_81_0.tar.gz && \
    tar zxvf /boost_1_81_0.tar.gz -C /
WORKDIR /boost_1_81_0
RUN \
    /boost_1_81_0/bootstrap.sh --prefix=/usr/local && \
    /boost_1_81_0/b2 install --prefix=/usr/local; exit 0
WORKDIR /

RUN \
    wget https://github.com/pocoproject/poco/archive/refs/tags/poco-1.12.4-release.zip -O /poco-1.12.4-release.zip && \
    unzip /poco-1.12.4-release.zip -d / && \
    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=NO -G "Ninja" -S /poco-poco-1.12.4-release -B /poco-poco-1.12.4-release/build && \
    cmake --build /poco-poco-1.12.4-release/build && \
    cmake --install /poco-poco-1.12.4-release/build

RUN \
   wget https://github.com/google/glog/archive/refs/tags/v0.6.0.tar.gz -O /glog.tar.gz && \
    tar zxvf /glog.tar.gz -C / && \
    cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" -S /glog-0.6.0 -B /glog-0.6.0/build && \
    cmake --build /glog-0.6.0/build && \
    cmake --build /glog-0.6.0/build --target install

RUN \
   wget https://github.com/google/benchmark/archive/refs/tags/v1.7.1.tar.gz -O /benchmark.tar.gz && \
    tar zxvf /benchmark.tar.gz -C / && \
    cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -G "Ninja" -S /benchmark-1.7.1 -B /benchmark-1.7.1/build && \
    cmake --build /benchmark-1.7.1/build --config Release && \
    cmake -E chdir /benchmark-1.7.1/build ctest --build-config Release && \
    cmake --build /benchmark-1.7.1/build --config Release --target install

RUN \
   wget https://github.com/jtv/libpqxx/archive/refs/tags/7.7.4.tar.gz -O /libpqxx.tar.gz && \
    tar zxvf /libpqxx.tar.gz -C / && \
    cmake -G "Ninja" -S /libpqxx-7.7.4 -B /libpqxx-7.7.4/build -DCMAKE_PREFIX_PATH=usr/lib/cmake -DCMAKE_BUILD_TYPE=Release && \
    cmake --build /libpqxx-7.7.4/build --config Release && \
    cmake --build /libpqxx-7.7.4/build --config Release --target install

RUN \
   wget https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz -O /googletest.tar.gz && \
    tar zxvf /googletest.tar.gz -C / && \
    cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" -S /googletest-1.13.0 -B /googletest-1.13.0/build && \
    cmake --build /googletest-1.13.0/build && \
    cmake -E chdir /googletest-1.13.0/build ctest --build-config Release && \
    cmake --build /googletest-1.13.0/build --target install

RUN \
   wget https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz -O /nlohmann.tar.gz && \
    tar zxvf /nlohmann.tar.gz -C / && \
    cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" -S /json-3.11.2 -B /json-3.11.2/build && \
    cmake --build /json-3.11.2/build && \
    cmake --build /json-3.11.2/build --target install

RUN \
    pip install cpplint
