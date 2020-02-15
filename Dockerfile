FROM ubuntu:18.04

RUN apt update && apt install -y libgumbo1 libcurl3-gnutls

COPY ./bin/auto-ads-parser /usr/bin/
COPY ./example/config.json /etc/auto-ads-parser/conf/

VOLUME ["/var/log/auto-ads-parser"]
EXPOSE 8080
ENTRYPOINT [ "auto-ads-parser", "/etc/auto-ads-parser/conf/config.json"]