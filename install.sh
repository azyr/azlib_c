#!/bin/sh

./Build_ename.sh  > ename.c.inc
gcc -c -g3 -Wall -Werror -std=c99 *.c
rm libaz.a
ar rs libaz.a *.o
mkdir -p ${HOME}/lib
cp libaz.a ${HOME}/lib
mkdir -p ${HOME}/include/azlib
cp *.h ${HOME}/include/azlib
