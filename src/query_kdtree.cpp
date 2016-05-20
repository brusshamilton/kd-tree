#include <fstream>
#include <iostream>
#include <string>
#include "kdtree.h"
#include "utilities.h"

int main(int argc, const char* argv[])
{
  std::string filename = "tree.csv";

  std::ifstream is(filename);
  if (!is)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  
  kdtree::kdTree<double> tree;
  is >> tree;
  
#ifdef DEBUG
  filename = "tree2.csv";
  std::ofstream os(filename);
  if (!os)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  
  os << tree;
#endif
  
  return 0;
}
