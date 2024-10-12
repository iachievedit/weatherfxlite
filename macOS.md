# Building on macOS

To build on macOS you'll need to install Homebrew and then install the necessary dependencies:

```
brew install qt5
brew install openssl
```

You may need to update your `PATH`:

```
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
```

Then:

```
OPENSSL_PREFIX=`brew --prefix openssl`;
qmake INCLUDEPATH+="$OPENSSL_PREFIX/include" \
      LIBS+="-L$OPENSSL_PREFIX/lib -lcrypto"
make
```

To run the application from the command line:

```
% build/weatherfxLite.app/Contents/MacOS/weatherfxLite