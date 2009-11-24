#!/bin/bash

BINARY="$1"
TARGET="$2"

APP_BUNDLE="Dubnium.app"

rm -rf "$TARGET"
mkdir -p "$TARGET"
cp -r "mac/$APP_BUNDLE/Contents" "$TARGET"
mkdir -p "$TARGET/Contents/MacOS" >& /dev/null
cp -r images/*png "$TARGET/Contents/Resources/"
find "$TARGET" -name .svn -a -type d -exec rm -rf \{\} \; >& /dev/null
cp "$BINARY" "$TARGET/Contents/MacOS/"
