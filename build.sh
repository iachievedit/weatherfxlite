#!/bin/bash

# https://stackoverflow.com/questions/394230/how-to-detect-the-os-from-a-bash-script
case "$OSTYPE" in
  solaris*) echo "SOLARIS" ;;
  darwin*)  echo "Build for macOS";
  OPENSSL_PREFIX=`brew --prefix openssl`;
  ;;

  linux*)   echo "Build for Linux";
  OPENSSL_PREFIX="/usr/include";
  
  ;;
  bsd*)     echo "BSD" ;;
  msys*)    echo "WINDOWS" ;;
  cygwin*)  echo "ALSO WINDOWS" ;;
  *)        echo "unknown: $OSTYPE" ;;
esac

qmake INCLUDEPATH+="$OPENSSL_PREFIX/include" LIBS+="-L$OPENSSL_PREFIX/lib -lcrypto"
make -j4