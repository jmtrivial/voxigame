#!/usr/bin/env bash

VG2MANUAL=src/tools/vg2manual
VGDIRECTORY=../examples/patterns/
MANUALDIRECTORY=examples/patterns/

if [ ! -e "$MANUALDIRECTORY" ]; then
    mkdir examples/
    mkdir examples/patterns/
fi

$VG2MANUAL $VGDIRECTORY/tunnel.vg  $MANUALDIRECTORY/tunnel.pdf -a "L. Provot" -c -l 1 -n "Tunnel" -i 1 -d "28.03.2011"

$VG2MANUAL $VGDIRECTORY/bend.vg  $MANUALDIRECTORY/bend.pdf -a "V. Barra, J.-M. Favreau" -c -l 2 -n "Bend" -i 2 -d "05.04.2011" --nb-columns 3

$VG2MANUAL $VGDIRECTORY/armchair.vg  $MANUALDIRECTORY/armchair.pdf -a "J.-M. Favreau" -c -l 3 -n "Armchair" -i 3 -d "09.06.2011"

$VG2MANUAL $VGDIRECTORY/turn.vg  $MANUALDIRECTORY/turn.pdf -a "L. Provot, J.-M. Favreau" -c -l 3 -n "Turn" -i 4 -d "09.06.2011"

$VG2MANUAL $VGDIRECTORY/corner.vg  $MANUALDIRECTORY/corner.pdf -a "L. Provot, J.-M. Favreau" -c -l 2 -n "Corner" -i 5 -d "09.06.2011"

$VG2MANUAL $VGDIRECTORY/diagonal.vg  $MANUALDIRECTORY/diagonal.pdf -a "L. Provot, J.-M. Favreau" -c -l 4 -n "Diagonal" -i 6 -d "09.06.2011"
