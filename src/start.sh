#!/bin/bash

option=$1

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/app/anole"
# DEBUG
# export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/zlog-1.2.15/lib:/usr/local/boost_1_66_0/lib"

# maybe soft link
cwd=$(dirname $0)

function start()
{
    cd ${cwd}
    ${cwd}/anole $option
}

start $*
