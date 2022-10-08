#!/bin/bash

case "$OSTYPE" in
  solaris*) echo "SOLARIS" ;;
  darwin*)  echo "OSX"

  echo "Mac";
  OPENSSL_PREFIX=`brew --prefix openssl`;
  ;;

  linux*)   echo "LINUX"
  
  echo "Hi!";

  ;;
  bsd*)     echo "BSD" ;;
  msys*)    echo "WINDOWS" ;;
  cygwin*)  echo "ALSO WINDOWS" ;;
  *)        echo "unknown: $OSTYPE" ;;
esac

qmake INCLUDEPATH+="$OPENSSL_PREFIX/include" LIBS+="-L$OPENSSL_PREFIX/lib -lcrypto"
make