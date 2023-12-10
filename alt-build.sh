#!/bin/bash

wget https://ftp.gnu.org/gnu/ncurses/ncurses-6.4.tar.gz
tar -xf ncurses-6.4.tar.gz
cd ncurses-6.4
CPPFLAGS="-P" ./configure --prefix=$PWD/build
CPPFLAGS="-P" make
CPPFLAGS="-P" make install
cp build/include/ncurses/ncurses.h build/include/ncurses.h
cd ..
gcc -Incurces-6.4/build/include -Iinclude src/*.c -Lncurses-6.4/build/lib -lncurses -o backgammon
