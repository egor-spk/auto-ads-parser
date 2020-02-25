FROM ubuntu:18.04

RUN apt update && apt install -y libgumbo1 libcurl3-gnutls

COPY ./bin/parser-slave /usr/bin/
COPY ./slave/example/config.json /etc/parser-slave/conf/

VOLUME ["/var/log/parser-slave"]
EXPOSE 8080
ENTRYPOINT [ "parser-slave", "/etc/parser-slave/conf/config.json"]