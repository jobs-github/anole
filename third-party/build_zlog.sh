#!/bin/bash

test -d zlog-1.2.15 && rm -fr zlog-1.2.15
tar -zxvf zlog-1.2.15.tar.gz
cd zlog-1.2.15
make PREFIX=/usr/local/zlog-1.2.15 install
cd ..
rm -fr zlog-1.2.15
