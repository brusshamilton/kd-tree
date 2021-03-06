Main Page {#mainpage}
=========

kdtree - A generic kd-tree implementation

## Requirements

- cmake>=2.8.3 is required for building this project
- doxygen>=1.8.3 is required for building project documentation. 

## Building

Use the command `cmake .` from the project root directory to construct the Makefile's.
Then use the command `make` to build the executables

### Build Options

 - Pass `-DBUILD_DOCUMENTATION=1` to cmake to enable documentation.
 - Pass `-DBUILD_TEST=1` to cmake to enable compilation of a brute-force nearest neighbor search executable that is used to verify the kdtree implementation is generating the correct results.

## Executables

This project include two sample binaries using the kdtree framework. 
1. `build_tree` reads an input file (a set of points to use to build the tree, with the format described below) from the file named "sample_data.csv" in the current working directory, constructs a kd-tree and serializes it into the file "tree.csv" in the current working directory.
2. `query_tree` reads an input file (a set of points to find the nearest neighbor of) from the file named "query_data.csv" in the current working directory. It also reads a serialized tree data from the file "tree.csv". This program then writes to the output file name "output.csv" in the current working directory (with the format described below).

## Data format
The input data is assumed to be contained in a file named "sample_data.csv" located in the current working directory. This file is assumed to consist of comma ',' separated numbers, with each line representing a different vector and all the elements of a vector in a single line. The parser is not very smart and will probably silently fail if the input doesn't match what was expected.

The serialized tree data is stored in a file named "tree.csv" located in the current working directory. Its format starts with a block which contains a copy of the input data followed by an empty line. Next comes the 0-based row index in the data corresponding to the root of the tree followed by an empty line. Ater that there should be one line for each node in the tree (which should be the same as the number of input data). Each line has space separated values representing the node's index, the axis to split over, and the index of the left and right children.

The query data is assumed to be contained in a file named "query_data.csv" located in the current working directory. This file is assumed to consist of comma ',' separated numbers, with each line representing a different vector and all the elements of a vector in a single line. The parser is not very smart and will probably silently fail if the input doesn't match what was expected.

The output data is placed in a file named "output.csv" in the current working directory. It consists of one line for each line of the query data with the index of nearest neighbor to the corresponding input query followed by a space and the the distance between the query and the nearest neighbor.

## Testing and verification

This package also can generate an auxiliary executable `bruteforcesolver` which uses a brute-force algorithm to find the nearest neighbor. This executable takes the same input format for the input data as `build_tree` and the same query data as `query_tree`. It also outputs the result in the same format as `query_tree`. This makes `bruteforcesolver` functionally equivalent to running `build_tree` and then `query_tree`, but as it does not use optimized data structures it will run slower. It provides not only provides a useful reference for testing, but also serves as a baseline for comparison to evaluate kdtree performance.

