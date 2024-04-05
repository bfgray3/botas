#!/usr/bin/env bash

echo scipy
/bin/time python script.py $1 $2
make main
echo botas
/bin/time ./main $1 $2
