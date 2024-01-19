FROM alpine:latest

RUN apk add --no-cache \
    git \
    build-base \
    g++ \
    cmake \
    clang \
    postgresql-dev \
    postgresql \
    postgresql-contrib \
    argon2-dev \
    openssl-dev \
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
    cmake .. -DCMAKE_BUILD_TYPE=Release -DOATPP_BUILD_TESTS=OFF && \
    make -j "$(nproc)" install && \
    cd ../../; \
} && \
\
installmodule oatpp && \
installmodule oatpp-swagger && \
installmodule oatpp-postgresql && \
rm -rf /tmp/oatpp*

# Setup postgresql to be available at postgresql://cecil:password@localhost:5432/tda
RUN mkdir -p /var/lib/postgresql/data && \
    mkdir -p /run/postgresql && \
    chown -R postgres:postgres /run/postgresql && \
    chown -R postgres:postgres /var/lib/postgresql && \
    su postgres -c "initdb -D /var/lib/postgresql/data" && \
    su postgres -c "pg_ctl start -D /var/lib/postgresql/data -l /var/lib/postgresql/data/logfile" && \
    su postgres -c "psql -c \"CREATE USER cecil WITH PASSWORD 'password';\"" && \
    su postgres -c "psql -c \"CREATE DATABASE tda;\"" && \
    # Access to the database
    su postgres -c "psql -d tda -c \"GRANT ALL PRIVILEGES ON DATABASE tda TO cecil;\"" && \
    # Access to the tables
    su postgres -c "psql -d tda -c \"GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO cecil;\"" && \
    # Access to the schema
    su postgres -c "psql -d tda -c \"GRANT ALL PRIVILEGES ON SCHEMA public TO cecil;\"" && \
    # Access to the sequences
    su postgres -c "psql -d tda -c \"GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO cecil;\"" && \
    # Access to the functions
    su postgres -c "psql -d tda -c \"GRANT ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public TO cecil;\"" && \
    # Stop postgresql
    su postgres -c "pg_ctl stop -D /var/lib/postgresql/data"