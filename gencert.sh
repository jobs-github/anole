#!/bin/bash

function main()
{
    read -p "type domain: " domain
    SUBJECT="/C=US/ST=Mars/L=iTranswarp/O=iTranswarp/OU=iTranswarp/CN=$domain"
    echo "============================"
    echo "set password (5-10 digit)"
    echo "============================"
    openssl genrsa -des3 -out private.key 1024
    echo "============================"
    echo "type password you just set"
    echo "============================"
    openssl req -new -subj $SUBJECT -key private.key -out private.csr
    echo "============================"
    echo "type password again"
    echo "============================"
    mv private.key private.or.key
    openssl rsa -in private.or.key -out private.key
    openssl x509 -req -days 3650 -in private.csr -signkey private.key -out private.crt
}

main $*