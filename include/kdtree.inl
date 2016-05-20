#include <algorithm>
#include <vector>
#include <cassert>
#include <cmath>

namespace kdtree
{

  namespace
  {
    template<class T>
    class indirect_compare_kth
    {
    private:
      const std::vector<std::vector<T>> &values_;
      axis_t k_;
    public:
      indirect_compare_kth(const std::vector<std::vector<T>> &values, axis_t k) : values_(values), k_(k) {}
       
      bool operator()(size_t a, size_t b) 
      { 
        assert(values_[a].size() == values_[b].size());
        assert(values_[a].size() > k_);
        return values_[a][k_] < values_[b][k_];
      }
    };

  };

  template<class T>
  void kdTree<T>::buildTree(const std::vector<std::vector<T>> &values)
  {
    values_ = values;
  
    N_ = values_.size();     //how many points
    assert(N_ > 0);
    tree_.resize(N_);
    K_ = values_[0].size();  //how many dimensions
    
    std::vector<size_t> valid_idx;
    for (size_t n = 0; n < N_; n++)
      valid_idx.push_back(n);
    
    root_ = splitTree(valid_idx.begin(), valid_idx.end());
  }

  template<class T>
  size_t kdTree<T>::splitTree(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end)
  {
   if (start == end)
      return node::INVALID_IDX;
  
    std::pair<axis_t,std::vector<size_t>::iterator> splitpos = find_split(start,end);
    
    node n(splitpos.first,*splitpos.second);
    
    n.left()  = splitTree(start, splitpos.second);
    n.right() = splitTree(splitpos.second+1, end);
    
    tree_[n.idx()] = n;
    
    return n.idx();
  }
  
  template<class T>
  std::pair<axis_t,std::vector<size_t>::iterator> kdTree<T>::find_split(std::vector<size_t>::iterator start, std::vector<size_t>::iterator end)
  {
    axis_t split_axis = 0;
    T max_diff = 0;
    
    for (axis_t u = 0; u < K_; u++)
    {
      indirect_compare_kth<T> comp(values_, u);
      
      auto minmax = std::minmax_element(start, end, comp);
      
      T diff = values_[*(minmax.second)][u] - values_[*(minmax.first)][u];
      assert(diff >= 0);
      if (diff > max_diff)
      {
        split_axis = u;
        max_diff = diff;
      }
    }
    
    indirect_compare_kth<T> comp(values_, split_axis);
    std::sort(start, end, comp);
    
    std::vector<size_t>::iterator median = start + (end-start)/2;
    
    return std::pair<axis_t,std::vector<size_t>::iterator>(split_axis,median);

  }

  template<class T>
  size_t kdTree<T>::getNearest(const std::vector<T> &target)
  {
    //TODO
    return 0;
  }
  
  std::ostream & operator<<(std::ostream &os, const node& n)
  {
    os <<  n.axis_ << " " << n.idx_ << " " << n.left_ << " " << n.right_;
    return os;
  }
  
  std::istream & operator>>(std::istream &is, node& n)
  {
    is >> n.axis_ >> n.idx_ >> n.left_ >> n.right_;
    return is;
  }
  
};

