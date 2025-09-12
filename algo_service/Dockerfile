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
    pkg-config && \
    apt-get clean
RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git && \
    cd AMQP-CPP && \
    mkdir build && \
    cd build && \
    cmake .. -DAMQP-CPP_LINUX_TCP=ON && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf AMQP-CPP
RUN git clone https://github.com/libcpr/cpr.git && \
    cd cpr && \
    mkdir build && \
    cd build && \
    cmake .. -DCPR_USE_SYSTEM_CURL=ON && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf cpr
RUN git clone https://github.com/nlohmann/json.git && \
    cd json && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf json
WORKDIR /app
COPY . .
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)
EXPOSE 8082
