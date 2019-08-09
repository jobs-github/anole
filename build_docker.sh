#!/bin/bash

option=$1
cwd=$(dirname $(readlink -f $0))

image_runtime="anole:runtime"

function main()
{
    if test ! -z "$(docker images -q $image_runtime)"; then
        echo "$image_runtime exist"
    else
        docker image build -f third-party/Dockerfile_runtime -t $image_runtime .
    fi

    docker image build -t anole:0.0.1 .
}

main $*
