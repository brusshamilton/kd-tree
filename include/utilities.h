#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>
#include <iomanip>

namespace kdtree
{

  typedef size_t axis_t; //!< Axis indexing type
  
  const axis_t INVALID_AXIS = SIZE_MAX; //!< Sentinel value for invalid axis
  const size_t INVALID_IDX = SIZE_MAX;  //!< Sentinel value for invalid index

/** \addtogroup VectorIO Array of Vector I/O
 *  @{
 */


  /**
   * Utility function to read an array of points from a CSV formatted input stream
   */
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

  /**
   * Utility function to write an array of points to a CSV formatted output stream
   */
  template<class T>
  void writeValuesToCSV(std::ostream &os, const std::vector<std::vector<T>>& values)
  {
    os << std::scientific << std::setprecision(18);
    for (const std::vector<T>& row : values)
    {
      for (const T& val : row)
      {
        os << val << ",";
      }
      os << std::endl;
    }
  }
 
/** @}*/

/** \addtogroup VectorMath Vector Math calculations
 *  @{
 */
  /**
   * Calculate the norm of the difference between two vectors
   */
  template<class T>
  T normOfDifference(const std::vector<T>& a, const std::vector<T>& b, size_t L = 2)
  {
    T distL = 0;
    assert(a.size() == b.size());
    assert(L == 2); //only L2 norm supported for now
    for (kdtree::axis_t k = 0; k < a.size(); k++)
    {
      T tmp = a[k]-b[k];

      distL += tmp*tmp; //FIXME: only L2 norm is supported for now
    }
    return distL;
  }
  
  /**
   * Find the closest point to the target that still lies within the bounds
   *
   * \param[in]  target Target point
   * \param[in]  min    The lower corner of the hypercube defining the bounds
   * \param[in]  max    The upper corner of the hypercube defining the bounds
   * \param[out] result The output point
   */
  template<class T>
  void getClosestPointInBounds(const std::vector<T>& target, const std::vector<T>& min, const std::vector<T>& max, std::vector<T>& result)
  {
    result.resize(target.size());
    for (axis_t k = 0; k < target.size(); k++)
    {
      result[k] = std::max(std::min(target[k],max[k]),min[k]); //saturate to boundaries
    }
  }
/** @}*/
};

#endif

