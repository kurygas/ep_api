FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    libssl-dev \
    curl \
    libcurl4-openssl-dev \
    pkg-config \
    libasio-dev && \
    apt-get clean
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    mkdir build && \
    cd build && \
    cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf Crow
RUN git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DDISABLE_TESTS=ON && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf hireds && \
    ln -s /usr/local/lib/libhiredis.so /usr/lib/libhiredis.so.1 && \
    ldconfig
RUN git clone https://github.com/sewenew/redis-plus-plus.git && \
    cd redis-plus-plus && \
    mkdir build && \
    cd build && \
    cmake .. -DREDIS_PLUS_PLUS_BUILD_TEST=OFF && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf redis-plus-plus
RUN git clone https://github.com/Thalhammer/jwt-cpp && \
    cd jwt-cpp && \
    mkdir build && \
    cd build && \
    cmake .. -DJWT_BUILD_EXAMPLES=OFF && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf jwt-cpp
WORKDIR /app
COPY . .
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)
EXPOSE 8081
