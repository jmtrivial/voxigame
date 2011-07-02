#!/usr/bin/env bash

VG2MANUAL=src/tools/vg2manual
VGDIRECTORY=../examples/patterns/
MANUALDIRECTORY=examples/patterns/

if [ ! -e "$MANUALDIRECTORY" ]; then
    mkdir examples/
    mkdir examples/patterns/
fi

$VG2MANUAL $VGDIRECTORY/tunnel.vg  $MANUALDIRECTORY/tunnel.pdf -a "Laurent Provot" -c -l 1 -n "Tunnel" -i 1