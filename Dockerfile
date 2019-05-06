FROM ubuntu:18.04

ARG BRANCH=testing

LABEL maintainer="zaggash"

RUN \
  apt-get update && \
  apt-get install -y \
    git \
    libusb-1.0 \
    pkg-config

RUN \
  git clone -b ${BRANCH} https://github.com/audiohacked/OpenCorsairLink.git /OpenCorsairLink && \
  cd /OpenCorsairLink && \
  make

WORKDIR /OpenCorsairLink
ENTRYPOINT cp OpenCorsairLink.elf /opt
