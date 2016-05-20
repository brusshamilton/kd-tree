#include <fstream>
#include <iostream>
#include <string>
#include "kdtree.h"
#include "utilities.h"

template<class T>
kdtree::kdTree<T> loadTree(std::istream& is)
{
  std::vector<std::vector<T>> values = kdtree::loadValuesFromCSV<T>(is);
  
  size_t root;
  is >> root;

  std::vector<kdtree::node> nodes(values.size());
  for (kdtree::node& n : nodes)
  {
    is >> n;
  }

  return kdtree::kdTree<T>(values, root, nodes);
}

int main(int argc, const char* argv[])
{
  std::string filename = "tree.csv";

  std::ifstream is(filename);
  if (!is)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  
  kdtree::kdTree<double> tree = loadTree<double>(is);
  
#ifdef DEBUG
  filename = "tree2.csv";
  std::ofstream os(filename);
  if (!os)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  
  kdtree::writeValuesToCSV(os, tree.values());
  
  os << std::endl;
  
  os << tree.root() << std::endl;
  os << std::endl;
  
  for (const kdtree::node& n : tree.tree())
  {
    os << n << std::endl;
  }
#endif
  
  return 0;
}
