#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/bbos.kernel isodir/boot/bbos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "bbos" {
	multiboot /boot/bbos.kernel
}
EOF
grub-mkrescue -o bbos.iso isodir
