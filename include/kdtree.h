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
   * 
   */
  class node
  {
    public:
    
      node() : axis_(INVALID_AXIS), idx_(INVALID_IDX), left_(INVALID_IDX), right_(INVALID_IDX)
      {
      }
      
      /**
       * 
       */
      node(axis_t axis, size_t idx) : 
            axis_(axis), idx_(idx), left_(INVALID_IDX), right_(INVALID_IDX)
      {
      }
      
      ~node() { }
      
      const axis_t& axis() const { return axis_; }
      const size_t& idx() const { return idx_; }
      
      size_t& left() { return left_; } 
      size_t& right() { return right_; } 
      
    protected:
      axis_t axis_;
      size_t idx_;
      size_t left_, right_;
      
    private:
      friend std::ostream & operator<<(std::ostream &os, const node& n);
      friend std::istream & operator>>(std::istream &is, node& n);
      
  };

  template<class T>
  class kdTree
  {
    public:
       kdTree() : N_(0), K_(0) {}
       
       kdTree(const std::vector<std::vector<T>> &values) : N_(0), K_(0) 
       {
         buildTree(values);
       }
       
       kdTree(const std::vector<std::vector<T>> &values, size_t root, const std::vector<node>& tree) : values_(values), tree_(tree), root_(root)
       {
         N_ = values_.size();
         assert(N_ > 0);
         K_ = values_[0].size();
       }
       
      ~kdTree() {}
    
       void buildTree(const std::vector<std::vector<T>> &values);
       size_t getNearest(const std::vector<T> &target);

       const std::vector<std::vector<T>>& values() const { return values_; }
       const size_t& root() const { return root_; }
       const std::vector<node>& tree() const { return tree_; }
       
    protected:
       std::vector<std::vector<T>> values_;
       std::vector<node> tree_;
       size_t root_;
       
       size_t N_;
       axis_t K_;
       
       size_t splitTree(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end);
       std::pair<axis_t,std::vector<size_t>::iterator> find_split(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end);
       void getNearestBounded(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
       void getNearestBoundedLeft(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
       void getNearestBoundedRight(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest);
              
    private:
    
      template<class R>
      friend std::ostream & operator<<(std::ostream &os, const kdTree<R>& n);
      
      template<class R>
      friend std::istream & operator>>(std::istream &is, kdTree<R>& n);
  };
}; //namespace kdtree

#include <kdtree.inl>

#endif
