
#include <fstream>
#include <string>
#include "kdtree.h"
#include "utilities.h"

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
  
  os << tree;
    
  return 0;
}

