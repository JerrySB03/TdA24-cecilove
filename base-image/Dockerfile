FROM alpine:latest

RUN apk add --no-cache \
    git \
    build-base \
    g++ \
    cmake \
    clang \
    postgresql-dev \
    argon2-dev \
    sqlite-dev \
# Frontend 
    nodejs \
    npm \
    nginx \
# To run the server
    supervisor

RUN installmodule() { \
    echo "Downloading $1" && \
    git clone --depth=1 https://github.com/oatpp/$1 && \
    cd $1 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install && \
    cd ../../; \
} && \
\
installmodule oatpp && \
installmodule oatpp-swagger && \
installmodule oatpp-sqlite && \
rm -rf /tmp/oatpp*
