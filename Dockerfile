FROM ubuntu:16.04
MAINTAINER Mihail Fedorov <kolo@komodoplatform.com>

RUN apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install build-essential pkg-config libc6-dev m4 g++-multilib autoconf libtool ncurses-dev \
    unzip python zlib1g-dev wget bsdmainutils automake libssl-dev libprotobuf-dev \
    protobuf-compiler libqrencode-dev libdb++-dev software-properties-common libcurl4-openssl-dev curl && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ADD ./ /hush
ENV HOME /hush
WORKDIR /hush

# configure || true or it WILL halt
RUN cd /hush && \
    ./autogen.sh && \
    ./configure --with-incompatible-bdb --with-gui || true && \
    ./zcutil/build.sh -j$(nproc)

# Unknown stuff goes here

RUN ln -sf /hush/src/komodod /usr/bin/komodod && \
    ln -sf /hush/src/hushd /usr/bin/hushd && \
    ln -sf /hush/zcutil/docker-entrypoint.sh /usr/bin/entrypoint && \
    ln -sf /hush/zcutil/docker-komodo-cli.sh /usr/bin/hush-cli

CMD ["entrypoint"]
