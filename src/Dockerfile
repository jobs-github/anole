FROM ubuntu:20.04.dev

COPY anole.tar.gz elf_3rd.tar.gz make_standalone.sh /tmp/

RUN cd /tmp \
 && tar -zxf elf_3rd.tar.gz -C /usr/local \
 && mkdir -p anole/logs anole/conf workspace \
 && mv anole.tar.gz workspace \
 && cd workspace \
 && tar -zxf anole.tar.gz \
 && chmod a+x prebuild.sh \
 && ./prebuild.sh \
 && ./build.sh \
 && cd src \
 && cp ldd.txt /tmp \
 && chmod +x start.sh \
 && cp anole start.sh /tmp/anole/ \
 && cd /tmp \
 && chmod a+x make_standalone.sh \
 && ./make_standalone.sh