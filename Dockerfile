FROM ubuntu

RUN apt-get update
RUN apt-get install -y gcc make telnet net-tools clang-format curl valgrind git
