# Interprocess Communication Driver

## Purpose
Developed an IPC driver to explore Linux multi-processing for the creation of an undirected graph and shortest path applications.

## Definitions
**Undirected Graph:** a set of objects (called vertices or nodes) that are connected together, where all the edges are bidirectional.

**Vertice:** the fundamental unit by which graphs are formed.

**Edge:** a bidirectional line which connects an unordered pair of vertices together.

## Project Setup
To initialize the build sequence:
```
$ cd Interprocess_Communication_Driver && mkdir build && cd build && cmake ../ && make install 
```
To run the project:
```
$ cd Interprocess_Communication_Driver/build/run/bin/
./start
```

## Commands
This program accepts command-line arguments which affects the types of graphs generated. An execution instance portraying the use of all command line arguments is portrayed below:
```
./start -s 5 -n 4 -l 5 -c 20
```
**Where:**

**-s # :** # is an integer greater than or equal to 2. The number of streets randomly generated will be an integer in [2,#]. If this option is not specified, the default value is 10; that is, the number of streets randomly generated will be an integer in [2,10].

**-n # :** # is an integer greater than or equal to 1. The number of line segments randomly generated in each street will be an integer in [1,#]. Default: # = 5.

**-l # :** # is an integer greater than or equal to 5. The process will wait a random number of seconds in [5,#] before generating the next random input. Default: # = 5.

**-c # :** # is an integer greater than or equal to 1. The process will generate (x,y) coordinates such that every x and y value is in the range [-#,#]. Default: # = 20.

## Example
**Input:**
```
./start -s 5 -n 4 -l 5 -c 20
```

**Output:**
```
V 20
E {<0,1>,<1,2>,<2,3>,<3,4>,<4,5>,<5,6>,<7,8>,<8,4>,<4,9>,<9,10>,<10,11>,<11,12>,<12,2>,<2,13>,<13,1>,<1,14>,<15,5>,<5,16>,<16,6>,<6,17>,<17,9>,<9,11>,<11,18>,<18,12>,<12,3>,<3,8>,<8,19>}

V 25
E {<0,1>,<1,2>,<2,3>,<3,4>,<4,5>,<5,6>,<6,7>,<7,8>,<9,1>,<1,10>,<10,5>,<5,11>,<11,12>,<12,13>,<13,14>,<14,15>,<16,10>,<10,17>,<17,7>,<7,18>,<19,14>,<14,12>,<12,20>,<21,4>,<4,2>,<2,22>,<23,17>,<17,6>,<6,11>,<11,24>}

V 15
E {<0,1>,<1,2>,<2,3>,<3,4>,<5,6>,<6,7>,<8,3>,<3,9>,<9,10>,<10,11>,<12,6>,<6,9>,<9,2>,<2,13>,<13,1>,<1,10>,<10,14>}
.
.
.
```


