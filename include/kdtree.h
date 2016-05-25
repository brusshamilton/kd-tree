#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <cassert>

#include "utilities.h"

namespace kdtree
{
  /**
   * Class representing a structural element of the kdtree
   */
  class node
  {
    public:
    
      /**
       * \brief Default Constructor => creates an invalid node
       *
       */
      node() : axis_(INVALID_AXIS), idx_(INVALID_IDX), left_(INVALID_IDX), right_(INVALID_IDX)
      {
      }
      
      /**
       * \brief Constructs a node that separates the space
       * \param[in] axis Which axis we are separating the space over
       * \param[in] idx  Which index in the array of points does this node represent
       */
      node(axis_t axis, size_t idx) : 
            axis_(axis), idx_(idx), left_(INVALID_IDX), right_(INVALID_IDX)
      {
      }
      
      /**
       * Default destructor
       */
      ~node() { }
      
      //! accessor for the axis_ member (read only)
      const axis_t& axis() const { return axis_; }
      
      //! accessor for the idx_ member (read only)
      const size_t& idx() const { return idx_; }
      
      //! accessor for the left_ member
      size_t& left() { return left_; } 
      
      //! accessor for the right_ member
      size_t& right() { return right_; } 
      
    protected:
      axis_t axis_; //!< Axis that this node separates the space over
      size_t idx_;  //!< Index in the array of points that this node represents
      size_t left_; //!< Left (<= split point) child of this node
      size_t right_; //!< Right (>= split point) child of this node
      
    private:
      /**
       * Function to serialize a node to a stream
       */
      friend std::ostream & operator<<(std::ostream &os, const node& n);
      
      /**
       * Function to deserialize a node from a stream
       */
      friend std::istream & operator>>(std::istream &is, node& n);
      
  };

  /**
   * Class implementing a kd-tree
   */
  template<class T>
  class kdTree
  {
    public:
      /**
       * \brief Default Constructor => creates an invalid kdTree
       *
       */
      kdTree() : N_(0), K_(0) {}
       
      /**
       * \brief Creates a kdTree from an array of points
       *
       * \param[in] values The array of points to use when constructing the tree
       */
      kdTree(const std::vector<std::vector<T>> &values) : N_(0), K_(0) 
      {
        buildTree(values);
      }

      /**
       * Default destructor
       */
      ~kdTree() {}
    
      /**
       * \brief Initializes a kdTree from an array of points
       *
       * \param[in] values The array of points to use when constructing the tree
       */    
      void buildTree(const std::vector<std::vector<T>> &values);
      
      /**
       * \brief Find the nearest point in the tree to the target point
       *
       * \param[in] target The k-dimensional vector representing the target point
       *
       * \return Index in the tree's array of points for the nearest point
       */    
      size_t getNearest(const std::vector<T> &target);

      //! accessor for the values member (read only)
      const std::vector<std::vector<T>>& values() const { return values_; }
      
      //! accessor for the root member (read only)
      const size_t& root() const { return root_; }
      
      //! accessor for the tree member (read only)
      const std::vector<node>& tree() const { return tree_; }
       
    protected:
      std::vector<std::vector<T>> values_; //!< Array of k-dimensional points that were used to construct this tree
      std::vector<node> tree_; //!< Array of node's corresponding to the tree structure
      size_t root_; //!< Index of the node at the top of the tree
       
      size_t N_; //!< Number of points stored in this tree
      axis_t K_; //!< Number of dimensions used in this tree
      
      
      /**
       * \brief Creates a tree from a list of points
       *
       * This function is called recursively to create a the tree from a list of points.
       * It uses the find_split function to determine the position of the split, and then
       * recurses to create trees to the left and right of the split
       *
       * \warning This function may reorder the list of points passed as its input arguments
       *
       * \param[in] start Beginning of the list of points
       * \param[in] end   End of the list of points
       *
       * \return Index in the tree's array for this node
       */
      size_t splitTree(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end);
      
      /**
       * \brief Finds the point and axis where the tree should split
       *
       * This function finds the axis with the largest range and then chooses the median value as the split position.
       *
       * \warning This function may reorder the list of points passed as its input arguments
       *
       * \param[in] start Beginning of the list of points
       * \param[in] end   End of the list of points
       *
       * \return std::pair containing the axis to split over and an iterator to the element to split at
       */
      std::pair<axis_t,std::vector<size_t>::iterator> find_split(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end);
      
      /**
       * \brief Finds the closest point in the subtree
       *
       * This function finds the closest point (if any) in the subtree  
       *
       *
       * \param[in]     target       Point that we need to find the index of
       * \param[in]     min_range    Point representing the lower corner of hyper-cube bounding the search area
       * \param[in]     max_range    Point representing the upper corner of hyper-cube bounding the search area
       * \param[in]     root         Root of the subtree
       * \param[in,out] closest_norm Distance of the current best estimate. The input is used as a bound.
       * \param[in,out] closest      Index of the point in the tree that is the current best estimate of the nearest neighbor
       */
      void getNearestBounded(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
      
      /**
       *  \copydoc getNearestBounded(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
       */
      void getNearestBoundedLeft(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
      
      /**
       *  \copydoc getNearestBounded(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
       */
      void getNearestBoundedRight(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
              
    private:
    
      /**
       * Function to serialize a kdTree to a stream
       */
      template<class R>
      friend std::ostream & operator<<(std::ostream &os, const kdTree<R>& n);

      /**
       * Function to deserialize a kdTree from a stream
       */      
      template<class R>
      friend std::istream & operator>>(std::istream &is, kdTree<R>& n);
  };
}; //namespace kdtree

#include <kdtree.inl>

#endif
