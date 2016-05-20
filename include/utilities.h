#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>

namespace kdtree
{

  template<class T>
  std::vector<std::vector<T>> loadValuesFromCSV(std::istream &is)
  {
    std::vector<std::vector<T>> retval;
    std::vector<T> tmp;
     
    while (is)
    {
      std::string line;
      std::getline(is,line);
      
      if (line.length() == 0)
      {
        break;
      }
      else
      {
        tmp.clear();
        std::stringstream ss(line);
        while(ss)
        {
          T val;
          char next = ss.peek();
          
          if ((next == '\n') || (next == ',') || next < 0)
            break;
          
          ss >> val;
          tmp.push_back(val);
          ss >> next;
          assert((next == ',') || !ss);
        }
        retval.push_back(tmp);
      }
    }
    return retval;
  }

  template<class T>
  void writeValuesToCSV(std::ostream &os, const std::vector<std::vector<T>>& values)
  {
    for (const std::vector<T>& row : values)
    {
      for (const T& val : row)
      {
        os << val << ",";
      }
      os << std::endl;
    }
  }
  
};

#endif

