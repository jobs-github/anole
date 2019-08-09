#!/bin/bash

test -d zlog-1.2.12 && rm -fr zlog-1.2.12
tar -zxvf zlog-1.2.12.tar.gz
cd zlog-1.2.12
make PREFIX=/usr/local/zlog-1.2.12 install
cd ..
rm -fr zlog-1.2.12
