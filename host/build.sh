#!/usr/bin/env bash
set -e

SRC="subscriber.c ../src/xor_cipher.c"
CFLAGS="-I../include -Wall -Wextra -O2"
LIBS="-lpaho-mqtt3c -lws2_32"   # winsock no Windows

echo "==> Compilando subscriber..."
gcc $CFLAGS $SRC $LIBS -o subscriber.exe
echo "==> Feito! Rode com ./subscriber.exe"
