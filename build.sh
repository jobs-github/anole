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

    # make    
    make
    python ldd.py anole /usr/ /lib/x86_64-linux-gnu/ /usr/local/lib/ /usr/local/boost_1_73_0/lib/
    # output anole & ldd.txt in src
}

main $*
