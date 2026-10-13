#!/bin/sh
set -e
make clean
make
make CPUS=1 qemu-nox
