FROM ubuntu:16.04

COPY installation/ /app/

RUN add-apt-repository -y ppa:nginx/stable \
 && apt-get update \
 && apt-get install nginx \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*