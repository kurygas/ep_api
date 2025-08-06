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
    libasio-dev
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../..
RUN git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../..
RUN git clone https://github.com/sewenew/redis-plus-plus.git && \
    cd redis-plus-plus && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../..
RUN git clone https://github.com/Thalhammer/jwt-cpp && \
    cd jwt-cpp && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../..
WORKDIR /app
COPY . .
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)
EXPOSE 8081
