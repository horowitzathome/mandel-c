FROM ubuntu:latest

RUN apt-get update && \
    apt-get -y install gcc libevent-dev && \
    apt-get clean

WORKDIR /app

COPY mandel.c .

CMD ["gcc", "-o", "mandel", "mandel.c", "-levent", "-I/usr/include/event2"]

EXPOSE 8080