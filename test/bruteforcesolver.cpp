#include <fstream>
#include <string>
#include <limits>
#include "kdtree.h"
#include "utilities.h"

int main(int argc, const char* argv[])
{
  std::vector<std::vector<double>> values, query;

  {
    std::string filename = "sample_data.csv";
    std::ifstream is(filename);
    if (!is)
    {
      std::cerr << "Error opening file: " << filename << std::endl;
      return -1;
    }
    values = std::move(kdtree::loadValuesFromCSV<double>(is));
  }
  
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
     std::string filename = "bf_output.csv";
     std::ofstream os(filename);
     if (!os)
     {
       std::cerr << "Error opening file: " << filename << std::endl;
       return -1;
     }
     
     for (const auto& q : query)
     {
       double mindist2 = std::numeric_limits<double>::infinity();
       size_t min_idx = kdtree::INVALID_IDX;
       
       for (size_t u = 0; u < values.size(); u++)
       {
         double dist2 = kdtree::normOfDifference(q,values[u]);
         if (dist2 < mindist2)
         {
           mindist2 = dist2;
           min_idx = u;
         }
       }
       os << min_idx << " " << sqrt(mindist2) << std::endl;
     }
  }    
  return 0;
}

