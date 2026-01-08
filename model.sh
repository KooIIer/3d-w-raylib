#!/bin/bash
python gen.py $1
gcc -l raylib -lm main.c -o main
./main $2 $3 $4 $5 $6 $7
