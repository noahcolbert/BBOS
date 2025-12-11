#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -m 4G -cdrom bbos.iso -monitor stdio
