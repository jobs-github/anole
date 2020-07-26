#!/bin/bash

function main()
{
    declare -a items=(\
        "build_zlog.sh" \
    )

    for item in "${items[@]}"
    do
        chmod a+x $item
        ./$item
    done
}

main $*