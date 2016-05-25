Main Page {#mainpage}
=========

kdtree - A generic kd-tree implementation

## Requirements

- cmake>=2.8.3 is required for building this project
- doxygen>=1.8.3 is required for building project documentation. 

## Build Options

 - Pass `-DBUILD_DOCUMENTATION=1` to cmake to enable documentation.
 - Pass `-DBUILD_TEST=1` to cmake to enable compilation of a brute-force nearest neighbor search executable that is used to verify the kdtree implementation is generating the correct results.

## Data format
The input data is assumed to be contained in a file named "sample_data.csv" located in the current working directory. This file is assumed to consist of comma ',' separated numbers, with each line representing a different vector and all the elements of a vector in a single line. The parser is not very smart and will probably silently fail if the input doesn't match what was expected.

The serialized tree data is stored in a file named "tree.csv" located in the current working directory. Its format starts with a block which contains a copy of the input data followed by an empty line. Next comes the 0-based row index in the data corresponding to the root of the tree followed by an empty line. Ater that there should be one line for each node in the tree (which should be the same as the number of input data). Each line has space separated values representing the node's index, the axis to split over, and the index of the left and right children.

The query data is assumed to be contained in a file named "query_data.csv" located in the current working directory. This file is assumed to consist of comma ',' separated numbers, with each line representing a different vector and all the elements of a vector in a single line. The parser is not very smart and will probably silently fail if the input doesn't match what was expected.

The output data is placed in a file named "output.csv" in the current working directory. It consists of one line for each line of the query data with the index of nearest neighbor to the corresponding input query followed by a space and the the distance between the query and the nearest neighbor.

