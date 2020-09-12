#!/usr/bin/env bash
# Copyright (c) 2019 The Hush developers
# Released under the GPLv3

set -eu -o pipefail

function cmd_pref() {
    if type -p "$2" > /dev/null; then
        eval "$1=$2"
    else
        eval "$1=$3"
    fi
}
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

# If a g-prefixed version of the command exists, use it preferentially.
function gprefix() {
    cmd_pref "$1" "g$2" "$2"
}

gprefix READLINK readlink
cd "$(dirname "$("$READLINK" -f "$0")")/.."

# Allow user overrides to $MAKE. Typical usage for users who need it:
#   MAKE=gmake ./zcutil/build.sh -j$(nproc)
if [[ -z "${MAKE-}" ]]; then
    MAKE=make
fi

# Allow overrides to $BUILD and $HOST for porters. Most users will not need it.
#   BUILD=i686-pc-linux-gnu ./zcutil/build.sh
if [[ -z "${BUILD-}" ]]; then
    BUILD="$(./depends/config.guess)"
fi
if [[ -z "${HOST-}" ]]; then
    HOST="$BUILD"
fi

# Allow users to set arbitrary compile flags. Most users will not need this.
if [[ -z "${CONFIGURE_FLAGS-}" ]]; then
    CONFIGURE_FLAGS=""
fi

if [ "x$*" = 'x--help' ]
then
    cat ./zcutil/dragon.txt
    cat <<EOF
Welcome To The Hush Build System, Here Be Dragons!
Usage:
$0 --help
  Show this help message and exit.
$0 [ --enable-lcov || --disable-tests ] [ --disable-mining ] [ --disable-libs ] [ MAKEARGS... ]
  Build Hush and most of its transitive dependencies from source. MAKEARGS are applied to both dependencies and Hush itself.
  If --enable-lcov is passed, Hush is configured to add coverage instrumentation, thus enabling "make cov" to work.
  If --disable-tests is passed instead, the Hush tests are not built.
  If --disable-mining is passed, Hush is configured to not build any mining code. It must be passed after the test arguments, if present.
  It must be passed after the test/mining arguments, if present.
EOF
    exit 0
fi

set -x

# If --enable-lcov is the first argument, enable lcov coverage support:
LCOV_ARG=''
HARDENING_ARG='--enable-hardening'
TEST_ARG=''
if [ "x${1:-}" = 'x--enable-lcov' ]
then
    LCOV_ARG='--enable-lcov'
    HARDENING_ARG='--disable-hardening'
    shift
elif [ "x${1:-}" = 'x--disable-tests' ]
then
    TEST_ARG='--enable-tests=no'
    shift
fi

# If --disable-mining is the next argument, disable mining code:
MINING_ARG=''
if [ "x${1:-}" = 'x--disable-mining' ]
then
    MINING_ARG='--enable-mining=no'
    shift
fi

# Just show the useful info
eval "$MAKE" --version | head -n2
as --version | head -n1
as --version | tail -n1
ld -v

HOST="$HOST" BUILD="$BUILD" "$MAKE" "$@" -C ./depends/ V=1

./autogen.sh

CONFIG_SITE="$PWD/depends/$HOST/share/config.site" ./configure "$HARDENING_ARG" "$LCOV_ARG" "$TEST_ARG" "$MINING_ARG" $CONFIGURE_FLAGS CXXFLAGS='-g'

#BUILD CCLIB

WD=$PWD
cd src/cc
echo $PWD
./makecustom
cd $WD

"$MAKE" "$@" V=1
