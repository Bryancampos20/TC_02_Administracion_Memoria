FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    nano \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY . .

RUN gcc -o MemoryManagement MemoryManagement.c

ENTRYPOINT ["./MemoryManagement"]
