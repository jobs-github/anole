FROM ubuntu:devel

COPY anole.tar.gz elf_3rd.tar.gz make_standalone.sh /tmp/

RUN cd /tmp \
 && tar -zxf elf_3rd.tar.gz -C /usr/local \
 && mkdir -p anole/logs workspace \
 && mv anole.tar.gz workspace \
 && cd workspace \
 && tar -zxf anole.tar.gz \
 && chmod a+x prebuild.sh \
 && ./prebuild.sh \
 && ./build.sh \
 && cd src \
 && cp ldd.txt /tmp \
 && cp anole /tmp/anole/ \
 && cd /tmp \
 && chmod a+x make_standalone.sh \
 && ./make_standalone.sh