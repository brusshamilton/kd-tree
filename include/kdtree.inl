#include <algorithm>
#include <vector>
#include <cassert>
#include <limits>

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
      return INVALID_IDX;
  
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
    std::vector<T> min_range(K_,std::numeric_limits<axis_t>::lowest());
    std::vector<T> max_range(K_,std::numeric_limits<axis_t>::max());
    
    T closest_norm = std::numeric_limits<T>::max();
    size_t closest = INVALID_IDX;
    
    //go through the tree
    getNearestBounded(target,min_range,max_range,root_,closest_norm,closest);
    
    return closest;
  }
  
  template<class T>
  void kdTree<T>::getNearestBounded(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest)
  {
#ifdef DEBUG
    for (axis_t k = 0; k < K_; k++) //tree consistency check -- vector must be in the expected subspace
    {
      assert(values_[root][k] >= min_range[k]);
      assert(values_[root][k] <= max_range[k]);
    }
#endif

    T norm = normOfDifference(target, values_[root]);
    if (norm < closest_norm)
    {
      closest_norm = norm;
      closest = root;
    }
    
    //heuristic: if target would be placed on left side, search there first
    if (target[tree_[root].axis()] < values_[root][tree_[root].axis()])
    {
      getNearestBoundedLeft(target,min_range,max_range,root,closest_norm,closest);
      getNearestBoundedRight(target,min_range,max_range,root,closest_norm,closest);
    } else
    {
      getNearestBoundedRight(target,min_range,max_range,root,closest_norm,closest);
      getNearestBoundedLeft(target,min_range,max_range,root,closest_norm,closest);
    }
  }
  
  template<class T>
  void kdTree<T>::getNearestBoundedLeft(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest)
  {
    if (tree_[root].left() != INVALID_IDX)
    {
      std::vector<T> new_min = min_range;
      std::vector<T> new_max = max_range;
      
      new_max[tree_[root].axis()] = values_[root][tree_[root].axis()];
      
      std::vector<T> tmp;
      getClosestPointInBounds(target, new_min, new_max, tmp);
      
      if (normOfDifference(target,tmp) < closest_norm) //if the closest point could possibly here
      {
        getNearestBounded(target, new_min, new_max, tree_[root].left(), closest_norm, closest);
      }
    }
  }
  
  template<class T>
  void kdTree<T>::getNearestBoundedRight(const std::vector<T> &target, const std::vector<T> &min_range, const std::vector<T> &max_range, size_t root, T& closest_norm, size_t& closest)
  {
    if (tree_[root].right() != INVALID_IDX)
    {
      std::vector<T> new_min = min_range;
      std::vector<T> new_max = max_range;
      
      new_min[tree_[root].axis()] = values_[root][tree_[root].axis()];
      
      std::vector<T> closest_in_bounds;
      getClosestPointInBounds(target, new_min, new_max, closest_in_bounds);
      
      if (normOfDifference(target,closest_in_bounds) < closest_norm) //if the closest point could possibly here
      {
        getNearestBounded(target, new_min, new_max, tree_[root].right(), closest_norm, closest);
      }
    }
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
  
  template<class T>
  std::ostream & operator<<(std::ostream& os, const kdtree::kdTree<T> &tree)
  {
    kdtree::writeValuesToCSV(os, tree.values());
  
    os << std::endl;
    
    os << tree.root() << std::endl;
    os << std::endl;
  
    for (const kdtree::node& n : tree.tree())
    {
      os << n << std::endl;
    }
    return os;
  }
  
  template<class T>
  std::istream & operator>>(std::istream& is, kdtree::kdTree<T> &tree)
  {
    tree.values_ = kdtree::loadValuesFromCSV<T>(is);
  
    tree.N_ = tree.values_.size();
    assert(tree.N_ > 0);
    tree.K_ = tree.values_[0].size();
  
    is >> tree.root_;

    tree.tree_.resize(tree.N_);
    for (kdtree::node& n : tree.tree_)
    {
      is >> n;
    }

    return is;
  }
};

