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

  std::vector<std::vector<double>> query;
  {
    std::string filename = "query_data.csv";
    std::ifstream is(filename);
    if (!is)
    {
      std::cerr << "Error opening file: " << filename << std::endl;
      return -1;
    }
    query = std::move(kdtree::loadValuesFromCSV<double>(is));
  }

  {
     std::string filename = "output.csv";
     std::ofstream os(filename);
     if (!os)
     {
       std::cerr << "Error opening file: " << filename << std::endl;
       return -1;
     }

#ifdef DEBUG     
     for (int i = 0; i < 10000; i++)
#endif
     for (const auto& q : query)
     {
       size_t min_idx = tree.getNearest(q);
       double mindist2 = kdtree::normOfDifference(q,tree.values()[min_idx]);

       os << min_idx << " " << sqrt(mindist2) << std::endl;
     }
  }    
  return 0;
}
