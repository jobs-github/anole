#!/bin/bash
cmd="/etc/init.d/nginx start && /app/anole/start.sh /app/anole/conf/server.json"
docker run --name anole --restart=always -d -p 80:80 -p 443:443 \
    -v /app/anole/conf:/app/anole/conf  \
    anole:0.0.1 \
    --mount type=bind,source=/home/$USER,target=/mnt/hgfs/share \
    sh -c $cmd