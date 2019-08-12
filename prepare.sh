#!/bin/bash

function main()
{
    mkdir -p /app/anole/conf /app/anole/logs
    cp gencert.sh src/server.json src/zlog.conf /app/anole/conf
    cd /app/anole/conf
    chmod +x gencert.sh
    ./gencert.sh
}

main $*