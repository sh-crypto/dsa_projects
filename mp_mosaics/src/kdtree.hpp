/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include <cmath>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    return (first[curDim] == second[curDim]) ? (first < second) : (first[curDim] < second[curDim]);
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double potential_mag = 0;
    double current_mag = 0;
    for (unsigned i = 0; i < Dim; i++) {
      potential_mag += ((potential[i] - target[i]) * (potential[i] - target[i]));
      current_mag += ((currentBest[i] - target[i]) * (currentBest[i] - target[i]));
    }
    return (potential_mag == current_mag) ? (potential < currentBest) : (potential_mag < current_mag);
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    if(newPoints.size() == 0) {
      size = 0;
      root = NULL;
    }
    auto vect = newPoints;
    root = KDTree_helper(vect, 0);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::KDTree_helper(vector<Point<Dim>>& newPoints, int dim) {
  if (newPoints.size() == 0) { return NULL; }
  if (newPoints.size() == 1) {
    KDTreeNode* subtree = new KDTreeNode(newPoints[0]);
    return subtree;
  }
  
  int median = floor((newPoints.size() - 1) / 2);
  auto cmp = [dim](const Point<Dim>& lhs, const Point<Dim>& rhs){return smallerDimVal(lhs, rhs, dim);};
  select(newPoints.begin(), newPoints.end(), (newPoints.begin() + median), cmp);
  KDTreeNode* node = new KDTreeNode(newPoints[median]);

  vector<Point<Dim>> left_tree (newPoints.begin(), newPoints.begin() + median);
  vector<Point<Dim>> right_tree (newPoints.begin() + median + 1, newPoints.end());

  node -> left = KDTree_helper(left_tree, (dim + 1) % Dim);
  node -> right = KDTree_helper(right_tree, (dim + 1) % Dim);
  return node;

}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy_(const KDTree<Dim>* other) {
  if (other == NULL) return NULL;
  KDTreeNode* node = new KDTreeNode(other->root->point);
  node -> right = copy_(other -> right);
  node -> left = copy_(other -> left);
  return node;
}


template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  size = other.size;
  if(other.root == NULL) {
    root = NULL;
    return;
  }
  root = new KDTree(other -> root);
  root -> left = copy_(other -> root -> left);
  root -> right = copy_(other -> root -> right);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (*this == rhs) { return *this; }
  if (root != NULL) {
    destroy_(root -> right);
    destroy_(root -> left);
    delete root;
    root = NULL;
  }
  if (rhs.root == NULL) {
    size = 0;
  } else {
    root = new KDTreeNode(rhs -> root);
    root -> left = copy_(rhs -> root -> left);
    root -> right = copy_(rhs -> root -> right);
    size = rhs.size();
  }
}

template <int Dim>
void KDTree<Dim>::destroy_(KDTreeNode* subroot) {
  if (subroot != NULL) {
    destroy_(subroot -> left);
    destroy_(subroot -> right);
    delete subroot;
    subroot = NULL;
  }
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  destroy_(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return findNearestNeighborHelper(root, query, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighborHelper(KDTreeNode* subroot, const Point<Dim>& query, int dim) const {  
  if (subroot == NULL) { return NULL; }
  if (subroot -> right == NULL && subroot -> left == NULL) { return subroot -> point; }
  Point<Dim> curr_point = subroot -> point;
  KDTreeNode* curr_node = subroot;

  

  bool left_or_not = false;
  if (smallerDimVal(query, curr_point, dim)) {
    curr_point = (curr_node -> left != NULL) ? findNearestNeighborHelper(curr_node -> left, query, ((dim + 1) % Dim)) : 
      findNearestNeighborHelper(curr_node -> right, query, ((dim + 1) % Dim));
    left_or_not = true;
  } else {
    curr_point = (curr_node -> right != NULL) ? findNearestNeighborHelper(curr_node -> right, query, ((dim + 1) % Dim)) :
      curr_point = findNearestNeighborHelper(curr_node -> left, query, ((dim + 1) % Dim));
    left_or_not = false;
  }

  
  
  if (shouldReplace(query, curr_point, curr_node -> point)) { curr_point = curr_node -> point; }

  double radius = getRadius(query, curr_point);
  double distance = pow((curr_node -> point)[dim] - query[dim], 2);

  if (radius < distance) { return curr_point; }
  KDTreeNode* tmp = (left_or_not) ? curr_node -> right : curr_node -> left;
  if (tmp == NULL) return curr_point;
  Point<Dim> tmp_pnt = findNearestNeighborHelper(tmp, query, ((dim + 1) % Dim));
  return shouldReplace(query, curr_point, tmp_pnt) ? tmp_pnt : curr_point;

}

template <int Dim>
double KDTree<Dim>::getRadius(const Point<Dim>& target, const Point<Dim>& currentBest) const {
    double magnitude = 0.0;
    for (unsigned i = 0; i < Dim; i++) {
      magnitude += pow((currentBest[i] - target[i]), 2);
    }
    return magnitude;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */
    if (start == end) { return; }
    else if (start == (end - 1)) { return; }
    auto pivot = std::distance(start, (end - 1)) / 2;
    int part = partition(start, end, pivot, cmp);

    auto check_k = std::distance(start, k);
    if (check_k < part) {
      select(start, (start + part), k, cmp);
    } else if (check_k > part) {
      select((start + part + 1), end, k, cmp);
    } else { return; }    
      
}

template <typename RandIter, typename Comparator>
int partition(RandIter start, RandIter end, int piv, Comparator cmp) {
  auto pivot_var = start;
  for (int i = 0; i < piv; i++) { pivot_var++; }

  auto last = end - 1;
  
  std::swap(*pivot_var, *last);
  auto tmp = start;
  for (auto i = start; i != last; i++) {
    if(cmp(*i, *last)) {
      std::swap(*tmp, *i);
      tmp++;
    }
  }
  std::swap(*last, *tmp);
  return std::distance(start, tmp);
}
