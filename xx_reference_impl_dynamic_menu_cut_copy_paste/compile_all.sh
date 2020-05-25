#!/bin/sh 

rm a.out

# add all files in the directory to files to compile
filelist=$(ls *.c)
#filelist=${1}

# "-g" is for "debug mode"
gcc -g `pkg-config --cflags gtk+-3.0` ${filelist} `pkg-config --libs gtk+-3.0`
#g++    `pkg-config --cflags gtk+-3.0` ${filelist} `pkg-config --libs gtk+-3.0
# -E = only preprocess
#gcc -E -g `pkg-config --cflags gtk+-3.0` dt_app.c `pkg-config --libs gtk+-3.0`` > preprocessed.c

echo "===================== compile finished ======================"
./a.out
