FROM ubuntu:20.04



RUN rm -rf /var/tez/build && mkdir -p /var/tez/build

COPY ./build /var/tez
ENV DEBIAN_FRONTEND=noninteractive

# Update apt and install development tools
RUN apt-get update


RUN apt-get install -y build-essential \
    cmake \
    libulfius-dev libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev libsystemd-dev\
    uwsc netcat \
    gcc make g++ sudo binutils tar gzip git


EXPOSE 8080


CMD ["tail", "-f", "/dev/null"]