FROM ubuntu:16.04

COPY installation/ /app/

RUN apt-get update \
 && apt-get install software-properties-common \
 && apt-get update \
 && add-apt-repository -y ppa:nginx/stable \
 && apt-get update \
 && apt-get install nginx \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*