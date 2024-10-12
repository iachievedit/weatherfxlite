#!/bin/bash

QMAKE_CXX="clang++"
QMAKE_LINK="clang++"

# https://stackoverflow.com/questions/394230/how-to-detect-the-os-from-a-bash-script
case "$OSTYPE" in
  solaris*) echo "SOLARIS" ;;
  darwin*)  echo "Build for macOS";

  OPENSSL_PREFIX=`brew --prefix openssl`;
  ;;

  linux*)   echo "Build for Linux";
  OPENSSL_PREFIX="/usr/include";
  sudo apt-get install -y qtbase5-dev libssl-dev clang

  ;;
  bsd*)     echo "BSD" ;;
  msys*)    echo "WINDOWS" ;;
  cygwin*)  echo "ALSO WINDOWS" ;;
  *)        echo "unknown: $OSTYPE" ;;
esac

if [ ! -f config.h ]; then
  echo "Error:  config.h not found."
  echo "See README.md for instructions on how to create config.h."
  exit 1
fi

qmake QMAKE_CXX="$QMAKE_CXX" QMAKE_LINK="$QMAKE_LINK" \
      INCLUDEPATH+="$OPENSSL_PREFIX/include" LIBS+="-L$OPENSSL_PREFIX/lib -lcrypto"

NUM_PROCESSORS=$(getconf _NPROCESSORS_ONLN)
make -j"$NUM_PROCESSORS"
