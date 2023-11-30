FROM jerrysb03/cecilove-base-image:0.0.2

COPY ./ /app

ENV DATABASE_CONNECTION=/app/data/cecilove.db

WORKDIR /app/backend/build

# Build backend and run it for 5 seconds to check if it starts
RUN set -eux; \
    mkdir -p /app/data; \
    cmake -DCMAKE_BUILD_TYPE=Release ..; \
    make -j $(nproc)

RUN set -eux; \
    timeout 5s /app/backend/build/cecilsky-backend-exe --stop; \
    exitcode=$?; \
    if [ $exitcode -eq 0 ]; then echo "Backend successfully built and started"; \
    elif [ $exitcode -eq 124 ]; then echo "Backend successfully built, but failed to stop, this may be due to invalid config"; \
    else echo "Backend failed to start"; exit 1; fi

COPY ./frontend /app/frontend

WORKDIR /app/frontend

# Build frontend
RUN set -eux; \
    npm install; \
    npm run build

# Nginx
COPY ./nginx/ /etc/nginx/

EXPOSE 80

ENV NODE_ENV=production

RUN cat /app/supervisord.conf \
    >> /etc/supervisord.conf

CMD ["/usr/bin/supervisord", "-n",  "-c", "/etc/supervisord.conf"]