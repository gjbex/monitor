#!/usr/bin/env bash

mkdir -p img bin
singularity build --fakeroot img/monitor.sif monitor.def

for command in $(grep apprun monitor.def | cut -d ' ' -f 2)
do
    cp scripts/command bin/$command
done
