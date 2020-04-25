# Interprocess Communication Driver

## Purpose
Developed an IPC driver to explore Linux multi-processing for the creation of randomized undirected graphs and application of the BFS shortest path. The driver pipes I/O's from a randomized street generator file along with code in the "Street_Map_Intersection_Identifier" and "Shortest_Path_Navigator" repositories to formulate its output.

## Definitions
**Undirected Graph:** a set of objects (called vertices or nodes) that are connected together, where all the edges are bidirectional.

**Vertice:** the fundamental unit by which graphs are formed.

**Edge:** a bidirectional line which connects an unordered pair of vertices together.

**Shortest Path:** the shortest possible path which can be traversed to reach one node from another.

## Project Setup
You must first install CMake to initiate the build sequence:
```
sudo apt install cmake
```
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

After the creation of a randomized undirected graph, you will have the aforementioned randomly generated amount of time to use the shortest path functionality for that graph **_before_** a new graph is generated:
```
s (first node) (second node)
```

## Example

**Input:**
```
./start -s 5 -n 4 -l 5 -c 20
```

**Output:**
```
V 19
E {<0,1>,<1,2>,<2,3>,<3,4>,<4,5>,<5,6>,<6,7>,<7,8>,<8,9>,<9,10>,<11,12>,<12,13>,<13,2>,<2,8>,<8,14>,<14,9>,<9,1>,<1,15>,<16,6>,<6,4>,<4,12>,<12,17>,<17,13>,<13,3>,<3,7>,<7,18>}
s 3 5
3-4-5

V 12
E {<0,1>,<1,2>,<2,3>,<4,5>,<5,6>,<7,8>,<8,1>,<1,9>,<10,8>,<8,2>,<2,5>,<5,11>}
s 5 7
3-2-5

V 20
E {<0,1>,<1,2>,<2,3>,<3,4>,<4,5>,<5,6>,<6,7>,<7,8>,<8,9>,<9,10>,<10,11>,<11,12>,<13,10>,<10,8>,<8,14>,<14,7>,<7,5>,<5,15>,<16,1>,<1,11>,<11,17>,<17,3>,<3,18>,<18,4>,<4,19>}
s 4 6
4-5-6
s 5 8
5-7-8
.
.
.
```


