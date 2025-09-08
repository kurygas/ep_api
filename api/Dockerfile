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
RUN git clone https://github.com/emweb/wt.git && \
    cd wt && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf wt
# Delete this after creating needed services
RUN git clone https://github.com/libcpr/cpr.git && \
    cd cpr && \
    mkdir build && \
    cd build && \
    cmake .. -DCPR_USE_SYSTEM_CURL=ON && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf cpr
RUN git clone https://github.com/Thalhammer/jwt-cpp.git && \
    cd jwt-cpp && \
    mkdir build && \
    cd build && \
    cmake .. -DJWT_BUILD_EXAMPLES=OFF && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf jwt-cpp
RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git && \
    cd AMQP-CPP && \
    mkdir build && \
    cd build && \
    cmake .. -DAMQP-CPP_LINUX_TCP=ON && \
    cmake --build . -j$(nproc) && \
    cmake --install . && \
    cd ../.. && \
    rm -rf AMQP-CPP
RUN mkdir -p /etc/wt && \
    echo '<?xml version="1.0" encoding="UTF-8"?>' > /etc/wt/wt_config.xml && \
    echo '<wt-config>' >> /etc/wt/wt_config.xml && \
    echo '  <debug-level>all</debug-level>' >> /etc/wt/wt_config.xml && \
    echo '</wt-config>' >> /etc/wt/wt_config.xml
WORKDIR /app
COPY . .
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)
EXPOSE 8080
