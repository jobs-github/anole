#!/bin/bash

function copy_3rd()
{
    libs=(
        "libboost_system.so.1.66.0 /usr/local/boost_1_66_0/lib/libboost_system.so.1.66.0"
        "libcrypto.so.1.1 /lib/x86_64-linux-gnu/libcrypto.so.1.1"
        "libssl.so.1.1 /lib/x86_64-linux-gnu/libssl.so.1.1"
        "libzlog.so.1.2 /usr/local/zlog-1.2.15/lib/libzlog.so.1.2"
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