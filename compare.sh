#!/usr/bin/env bash


if [[ $# -ne 2 ]]
then
  echo must pass N and NUM_REPLICATES as arguments >&2
  exit 1
fi

echo scipy
/bin/time python script.py "$1" "$2"
make main
echo botas
/bin/time ./main "$1" "$2"
