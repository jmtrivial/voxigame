# Voxigame


This software is a tool to manipulate 3D objects (pieces) in a 3D board.
It corresponds to an original puzzle game created by Jean-Marie Favreau 
in 2011. 

This software has been developped in 2011 under GPL (v3 or later), initially
using Qt4. In 2024, the source code has been ported for Qt5 and is able to
compile on a recent debian system.

## Description of the puzzle game

The aim of this puzzle is to arrange the bricks in a box in such a way that 
a path remains between the two windows on the sides of the box, while 
ensuring that the configuration is stable, i.e. that the bricks do not
move once the box is closed.

A detailled description of this puzzle game is available in french: see 
[règles (fr)](documentation/regles-fr.md) and
[conception (fr)](documentation/conception-fr.md).

A [blog entry](https://blog.jmtrivial.info/2024/08/12/voxigame/) from august 2024
add some personal context about this puzzle game.

## Dependancies

* cmake
* Qt5

## Usage

Use the following command to build the programs:

* ```mkdir build```
* ```cd build```
* ```cmake ..```
* ```make```

The main binary is ```src/tools/vg2manual```. Input of this program are 
xml files (with extension ```.vg```), and a script provide a standard 
conversion into manuals (in pdf format). From the ```build``` directory, 
run the following command:

* ```../scripts/patternManuals.sh```

It will generate a ```example``` directory with the corresponding manuals.

## Unit tests

Unit tests are then available in ```src/tests``` directory:

* ```cd src/tests```
* ```ctest```


## Authors

* [Jean-Marie Favreau](mailto:J-Marie.Favreau@uca.fr) ([Université Clermont Auvergne](http://uca.fr))
