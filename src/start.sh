#!/bin/bash

option=$1

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/app/anole"

# maybe soft link
cwd=$(dirname $0)

function start()
{
    cd ${cwd}
    ${cwd}/anole $option
}

start $*
