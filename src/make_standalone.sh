#!/bin/bash

function copy_3rd()
{
    libs=(
        "libboost_system.so.1.66.0 /usr/local/boost1.66/lib/libboost_system.so.1.66.0"
        "libcrypto.so.1.0.0 /lib/x86_64-linux-gnu/libcrypto.so.1.0.0"
        "libssl.so.1.0.0 /lib/x86_64-linux-gnu/libssl.so.1.0.0"
        "libzlog.so.1.1 /usr/local/zlog-1.2.15/lib/libzlog.so.1.1"
    )

    for lib in "${libs[@]}"
    do
        item=($lib)
        cp ${item[1]} ./${item[0]}
        chmod +x ./${item[0]}
    done
}

function main()
{
    cd /tmp/anole
    copy_3rd
    cd ..
    tar -zcf elf_anole.tar.gz anole
    rm -rf anole
}

main $*