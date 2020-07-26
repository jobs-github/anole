#!/bin/bash

function main()
{
    read -p "type domain: " domain
    SUBJECT="/C=US/ST=Mars/L=iTranswarp/O=iTranswarp/OU=iTranswarp/CN=$domain"

    openssl req -x509 -nodes -days 3650 \
        -newkey rsa:4096 -keyout ca_cert.key \
        -out ca_cert.pem -subj $SUBJECT
    openssl x509 -outform der -in ca_cert.pem -out ca_cert.der
}

main $*

#openssl s_server -CAfile ca_cert.pem -cert ca_cert.pem -key ca_cert.key -Verify 1
#openssl s_client -CAfile ca_cert.pem -cert ca_cert.pem -key ca_cert.key
