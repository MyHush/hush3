#!/bin/bash
# Copyright (c) 2019-2020 radix42
# Copyright (c) 2019-2020 The Hush developers
# Original aarch64 port by radix42. Thank you!

set -eu -o pipefail

cat <<'EOF'
 ________________
< Building Hush! >
 ----------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
EOF

if [ "x$*" = 'x--help' ]
then
    cat ./zcutil/dragon.txt
    cat <<EOF
Welcome To The Hush Build System, Here Be Dragons!
Usage:
$0 --help
  Show this help message and exit.
$0 [ --enable-lcov ] [ MAKEARGS... ]
  Build Hush and most of its transitive dependencies from
  source. MAKEARGS are applied to both dependencies and Hush itself. If
  --enable-lcov is passed, Hush is configured to add coverage
  instrumentation, thus enabling "make cov" to work.
EOF
    exit 0
fi

set -x
cd "$(dirname "$(readlink -f "$0")")/.."

# If --enable-lcov is the first argument, enable lcov coverage support:
LCOV_ARG=''
HARDENING_ARG='--disable-hardening'
if [ "x${1:-}" = 'x--enable-lcov' ]
then
    LCOV_ARG='--enable-lcov'
    HARDENING_ARG='--disable-hardening'
    shift
fi

# BUG: parameterize the platform/host directory:
PREFIX="$(pwd)/depends/aarch64-unknown-linux-gnu/"

HOST=aarch64-unknown-linux-gnu BUILD=aarch64-unknown-linux-gnu make "$@" -C ./depends/ V=1 NO_QT=1
./autogen.sh
CONFIG_SITE="$(pwd)/depends/aarch64-unknown-linux-gnu/share/config.site" ./configure --prefix="${PREFIX}" --host=aarch64-unknown-linux-gnu --build=aarch64-unknown-linux-gnu --with-gui=no --enable-rust=no "$HARDENING_ARG" "$LCOV_ARG" CXXFLAGS='-fwrapv -fno-strict-aliasing -g'

#BUILD CCLIB

WD=$PWD
cd src/cc
echo $PWD
./makecustom
cd $WD

make "$@" V=1
