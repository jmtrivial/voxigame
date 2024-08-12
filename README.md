# Voxigame

## Description

This software is a tool to manipulate 3D objects (pieces) in a 3D board.
It corresponds to an original puzzle game created by Jean-Marie Favreau 
in 2011. 

A detailled description of this puzzle game is available in french: see 
[règles (fr)](documentation/regles-fr.md).

This software is under GPL (v3 or later).

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
