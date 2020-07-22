#!/bin/bash

function test()
{
    docker run -it --name anole \
        -p 80:80 -p 443:443  \
        -v /app/anole/conf:/app/anole/conf  \
        -v /app/anole/logs:/app/anole/logs  \
        --mount type=bind,source=$HOME,target=/mnt/hgfs/share \
        anole:0.0.2 bash
}

function start()
{
    docker run --name anole --restart=always -d \
        -p 80:80 -p 443:443 \
        -v /app/anole/conf:/app/anole/conf  \
        -v /app/anole/logs:/app/anole/logs  \
        --mount type=bind,source=$HOME,target=/mnt/hgfs/share \
        anole:0.0.2 \
        sh -c "/etc/init.d/nginx start && /app/anole/start.sh /app/anole/conf/server.json"
} 

start $*