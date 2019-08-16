#!/bin/bash

option=$1
cwd=$(dirname $(readlink -f $0))

function clean()
{
    cd src
    make clean
}

function main()
{
    if [ "$option"x = "-c"x ]; then
        clean
        return 0
    fi

    echo "build..."
    cd src
    test -d build || (mkdir -p build)

    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib:/usr/local/boost1.66/lib"
    # make    
    make
}

main $*
