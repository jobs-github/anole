#!/bin/bash

function test()
{
    docker run -it \
        --name anole \
        -v /app/anole/conf:/app/anole/conf  \
        -v /app/anole/logs:/app/anole/logs  \
        --mount type=bind,source=/home/$USER,target=/mnt/hgfs/share \
        -p 8080:80 -p 4343:443  \
        anole:0.0.1 bash
}

function start()
{
    cmd="/etc/init.d/nginx start && /app/anole/start.sh /app/anole/conf/server.json"
    docker run --name anole --restart=always -d -p 80:80 -p 443:443 \
        -v /app/anole/conf:/app/anole/conf  \
        -v /app/anole/logs:/app/anole/logs  \
        anole:0.0.1 \
        --mount type=bind,source=/home/$USER,target=/mnt/hgfs/share \
        sh -c $cmd
} 

start $*