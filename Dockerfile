FROM ubuntu

RUN apt-get update
RUN apt-get install -y gcc make telnet
