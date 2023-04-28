FROM ubuntu:21.10

RUN apt-get update && \
    apt-get -y install gcc libevent-dev:2.1.12-stable && \
    apt-get clean

WORKDIR /app

COPY mandel.c .

CMD ["gcc", "-o", "mandel", "mandel.c", "-levent", "-I/usr/include/event2"]

EXPOSE 8080