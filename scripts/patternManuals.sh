#!/usr/bin/env bash

VG2MANUAL=src/tools/vg2manual
PVGDIRECTORY=../examples/patterns/
PMANUALDIRECTORY=examples/patterns/
VGDIRECTORY=../examples/
MANUALDIRECTORY=examples/

if [ ! -e "$PMANUALDIRECTORY" ]; then
    mkdir examples/
    mkdir examples/patterns/
fi

$VG2MANUAL $PVGDIRECTORY/tunnel.vg  $PMANUALDIRECTORY/tunnel.pdf -a "L. Provot" -c -l 1 -n "Tunnel" -i 1 -d "28.03.2011"

$VG2MANUAL $PVGDIRECTORY/bend.vg  $PMANUALDIRECTORY/bend.pdf -a "V. Barra, J.-M. Favreau" -c -l 2 -n "Bend" -i 2 -d "05.04.2011" --nb-columns 3

$VG2MANUAL $PVGDIRECTORY/armchair.vg  $PMANUALDIRECTORY/armchair.pdf -a "J.-M. Favreau" -c -l 3 -n "Armchair" -i 3 -d "09.06.2011"

$VG2MANUAL $PVGDIRECTORY/turn.vg  $PMANUALDIRECTORY/turn.pdf -a "L. Provot, J.-M. Favreau" -c -l 3 -n "Turn" -i 4 -d "09.06.2011"

$VG2MANUAL $PVGDIRECTORY/corner.vg  $PMANUALDIRECTORY/corner.pdf -a "L. Provot, J.-M. Favreau" -c -l 2 -n "Corner" -i 5 -d "09.06.2011"

$VG2MANUAL $PVGDIRECTORY/diagonal.vg  $PMANUALDIRECTORY/diagonal.pdf -a "L. Provot, J.-M. Favreau" -c -l 4 -n "Diagonal" -i 6 -d "09.06.2011"

$VG2MANUAL $PVGDIRECTORY/turn2.vg  $PMANUALDIRECTORY/turn2.pdf -a "Y. Gérard" -c -l 5 -n "Turn (2)" -i 7 -d "03.07.2011"

$VG2MANUAL $PVGDIRECTORY/throne.vg  $PMANUALDIRECTORY/throne.pdf -a "Y. Gérard" -c -l 3 -n "Throne" -i 3 -d "09.06.2011"

$VG2MANUAL $VGDIRECTORY/pipe1.vg $MANUALDIRECTORY/puzzle1.pdf -a "J.-M. Favreau" -c -l 1 -n "Puzzle 1" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/pipe2.vg $MANUALDIRECTORY/puzzle2.pdf -a "J.-M. Favreau" -c -l 3 -n "Puzzle 2" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/pipe3.vg $MANUALDIRECTORY/puzzle3.pdf -a "J.-M. Favreau" -c -l 5 -n "Puzzle 3" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/pipe4.vg $MANUALDIRECTORY/puzzle4.pdf -a "J.-M. Favreau" -c -l 6 -n "Puzzle 4" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/pipe5.vg $MANUALDIRECTORY/puzzle5.pdf -a "J.-M. Favreau" -c -l 7 -n "Puzzle 5" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/spiral.vg $MANUALDIRECTORY/spiral.pdf -a "J.-M. Favreau" -c -l 9 -n "Spiral" -d "12.06.2011"

$VG2MANUAL $VGDIRECTORY/spiral2.vg $MANUALDIRECTORY/spiral2.pdf -a "J.-M. Favreau" -c -l 10 -n "Spiral 2" -d "12.06.2011"