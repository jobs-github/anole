#!/bin/bash

option=$1
cwd=$(dirname $(readlink -f $0))

image_devel="ubuntu:20.04.dev"
image_3rd_name="ubuntu:anole-3rd-build"
container_3rd_name="anole-3rd-build"

image_srv_name="ubuntu:anole-build"
container_srv_name="anole-build"

dir_3rd=~/third-party/anole
name_3rd=elf_3rd.tar.gz
name_srv=elf_anole.tar.gz

function build_3rd()
{
    # build_tmp_3rd
    cd $cwd
    
    chmod a+x prebuild.sh
    ./prebuild.sh
    tar -zcf third-party.tar.gz third-party
    mv third-party.tar.gz third-party
    cd third-party

    if test ! -z "$(docker images -q $image_devel)"; then
        echo "$image_devel exist"
    else
        docker image build -f Dockerfile_devel -t $image_devel .
    fi

    if [ ! -f "boost_1_66_0.tar.gz" ]; then 
        wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
    else
        echo "boost_1_66_0.tar.gz exist"
    fi
    
    docker image build -f Dockerfile_3rd -t $image_3rd_name .

    docker run --name $container_3rd_name $image_3rd_name
    container_id=`docker ps -aqf "name=$container_3rd_name"`
    test -d $dir_3rd || mkdir -p $dir_3rd
    docker container cp $container_id:/tmp/$name_3rd $dir_3rd
    docker container kill $container_id
    docker container rm $container_id
    docker rmi -f $image_3rd_name
}

function build_srv()
{
    # build_tmp_srv
    cd $cwd

    cp $dir_3rd/$name_3rd src/
    mv anole.tar.gz src/
    cd src
    docker image build -t $image_srv_name .

    docker run --name $container_srv_name $image_srv_name
    container_id=`docker ps -aqf "name=$container_srv_name"`
    docker container cp $container_id:/tmp/$name_srv $cwd
    docker container cp $container_id:/tmp/ldd.txt $cwd
    docker container kill $container_id
    docker container rm $container_id
    docker rmi -f $image_srv_name

    cd $cwd
    mv ldd.txt src/
    test -d installation || mkdir -p installation
    mv $name_srv installation/
    cd installation
    tar -zxf $name_srv
    rm -f $name_srv
}

function main()
{
    git archive --format=tar HEAD | gzip > anole.tar.gz

    file_3rd=$dir_3rd/$name_3rd

    if [ ! -f "$file_3rd" ]; then 
        build_3rd
    else
        echo "$file_3rd exist"
    fi

    build_srv
}

main $*
