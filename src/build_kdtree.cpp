
#include <fstream>
#include <string>
#include "kdtree.h"
#include "utilities.h"

template<class T>
void writeTree(std::ostream& os, const kdtree::kdTree<T> &tree)
{
  kdtree::writeValuesToCSV(os, tree.values());
  
  os << std::endl;
  
  os << tree.root() << std::endl;
  os << std::endl;
  
  for (const kdtree::node& n : tree.tree())
  {
    os << n << std::endl;
  }
}

int main(int argc, const char* argv[])
{
  std::string filename = "sample_data.csv";

  std::ifstream is(filename);
  if (!is)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  kdtree::kdTree<double> tree(kdtree::loadValuesFromCSV<double>(is));
  
  filename = "tree.csv";
  std::ofstream os(filename);
  if (!os)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return -1;
  }
  
  writeTree<double>(os, tree);
    
  return 0;
}

