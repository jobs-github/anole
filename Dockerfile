FROM anole:runtime

COPY installation/ nginx /app/

RUN cd /app \
 && mv nginx /etc/