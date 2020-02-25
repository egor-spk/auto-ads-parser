FROM ubuntu:18.04

RUN apt update && apt install -y libgumbo1 libcurl3-gnutls

COPY ./bin/parser-master /usr/bin/
COPY ./master/example/config.json /etc/parser-master/conf/

VOLUME ["/var/log/parser-master"]
EXPOSE 8080
ENTRYPOINT ["parser-master", "/etc/parser-master/conf/config.json"]