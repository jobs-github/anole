#!/bin/bash

function copy_3rd()
{
    libs=(
        "libzlog.so.1.1 /usr/local/zlog-1.2.12/lib/libzlog.so.1.1"
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