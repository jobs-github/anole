FROM anole:runtime

COPY installation/ nginx.tar.gz /app/

RUN cd /app \
 && tar -zxf nginx.tar.gz -C /etc/ \
 && rm -f nginx.tar.gz

# Define mountable directories.
VOLUME ["/app/anole/conf"]
VOLUME ["/app/anole/logs"]

EXPOSE 80
EXPOSE 443