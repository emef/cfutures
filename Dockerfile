FROM ubuntu:trusty
MAINTAINER Benjamin Henrion <zoobab@gmail.com>

RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q --force-yes uuid-dev build-essential git-core libtool autotools-dev autoconf automake pkg-config unzip libkrb5-dev cmake

RUN useradd -d /home/zmq -m -s /bin/bash zmq
RUN echo "zmq ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/zmq
RUN chmod 0440 /etc/sudoers.d/zmq

USER zmq

WORKDIR /home/zmq
RUN git clone git://github.com/zeromq/cfutures.git
WORKDIR /home/zmq/cfutures
RUN mkdir build
WORKDIR /home/zmq/cfutures/build
RUN cmake ..
RUN make
RUN sudo make install
RUN sudo ldconfig
