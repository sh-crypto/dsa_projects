#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!(work_list.empty())) {
      work_list.pop_front();
    }
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!(work_list.empty())) {
      work_list.pop_front();
    }
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(work_list.empty()) {
      return Point();
    }
    return work_list.front();
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(work_list.empty()) {
      return Point();
    }
    return work_list.front();
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    root_ = start;
    png_ = png;
    tolerance_ = tolerance;
    fns_ = fns;
  }

  ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {
    root_ = start;
    tolerance_ = tolerance;
    png_ = png;
    fns_ = fns;
    fns_.add(work_list_, start);
    vp.resize(png_.height());
    for (auto &row : vp) {
        row.resize(png_.width(), false);
    }
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(png_, root_, tolerance_, fns_);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    if (work_list_.empty()) {
        //std::cout << "Work list is empty, ending traversal" << std::endl;
        return *this;
    }

    Point curr = fns_.peek(work_list_);
    //std::cout << "Current point: " << curr << std::endl;
    fns_.pop(work_list_);

    vp[curr.y][curr.x] = true;

    std::vector<Point> nearby = {};
    if (curr.x + 1 < png_.width() && !vp[curr.y][curr.x + 1]) nearby.emplace_back(curr.x + 1, curr.y);
    if (curr.y + 1 < png_.height() && !vp[curr.y + 1][curr.x]) nearby.emplace_back(curr.x, curr.y + 1);
    if (curr.x > 0 && !vp[curr.y][curr.x - 1]) nearby.emplace_back(curr.x - 1, curr.y);
    if (curr.y > 0 && !vp[curr.y - 1][curr.x]) nearby.emplace_back(curr.x, curr.y - 1);

    for (const Point& to_check : nearby) {
        double delta = calculateDelta(png_.getPixel(to_check.x, to_check.y), png_.getPixel(root_.x, root_.y));
        //std::cout << "Checking point: " << to_check << " with delta: " << delta << std::endl;

        if (delta <= tolerance_ && !vp[to_check.y][to_check.x]) {
            fns_.add(work_list_, to_check);
            //std::cout << "Adding point: " << to_check << " to work list with delta: " << delta << std::endl;
        } /*else {
            //std::cout << "Skipping point: " << to_check << " with delta: " << delta << std::endl;
        }*/
    }

    while (!work_list_.empty() && vp[fns_.peek(work_list_).y][fns_.peek(work_list_).x]) fns_.pop(work_list_);

    ////std::cout << "Work list size: " << work_list_.size() << std::endl;


    return *this;

  }


  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return fns_.peek(work_list_);
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if(empty() && other.empty()) return false;
    if(empty() != other.empty()) return true;
    std::deque<Point> work_list = other.work_list_;
    return !(fns_.peek(work_list_) == fns_.peek(work_list));
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }

}