FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \  
    gcc \             
    gdb \              
    nano \             
    && rm -rf /var/lib/apt/lists/*  

WORKDIR /usr/src/app

COPY . .

RUN gcc -o main src/main.c src/memory_management.c -Wall -Wextra -std=c11

RUN chmod +x main

ENTRYPOINT ["./main"]
