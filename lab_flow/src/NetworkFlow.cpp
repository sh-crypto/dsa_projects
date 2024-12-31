/**
 * @file NetworkFlow.cpp
 * CS 225: Data Structures
 */

#include <vector>
#include <algorithm>
#include <set>

#include "cs225_graph/graph.h"
#include "cs225_graph/edge.h"

#include "NetworkFlow.h"
using std::vector;
#include <iostream>

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  // YOUR CODE HERE
	for(Vertex v : startingGraph.getVertices()){ // copy over vertices
		residual_.insertVertex(v);
		flow_.insertVertex(v);
	}
	for(Edge e : startingGraph.getEdges()){ // copy over edges
		residual_.insertEdge(e.source, e.dest);
		residual_.setEdgeWeight(e.source, e.dest, startingGraph.getEdgeWeight(e.source, e.dest));
		residual_.insertEdge(e.dest, e.source); // reverse edge
		residual_.setEdgeWeight(e.dest, e.source, 0);
		flow_.insertEdge(e.source, e.dest);
		flow_.setEdgeWeight(e.source, e.dest, 0);
	}
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @@params: source -- The starting (current) vertex
   * @@params: sink   -- The destination vertex
   * @@params: path   -- The vertices in the path
   * @@params: visited -- A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @@params: source -- The starting (current) vertex
   * @@params: sink   -- The destination vertex
   * @@params: path   -- The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @@params: path   -- The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  // YOUR CODE HERE
	int min = residual_.getEdgeWeight(path.at(0), path.at(1));
	for(unsigned i = 1; i < path.size() - 1; i++){
		int weight = residual_.getEdgeWeight(path.at(i), path.at(i+1));
		min = weight < min ? weight : min;
	}
	return min;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @@outputs: The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
	vector<Vertex> path;
	while(findAugmentingPath(source_, sink_, path)){
		int cap = pathCapacity(path);
		for(unsigned i = 0; i < path.size() - 1; i++){
			Vertex s = path.at(i);
			Vertex d = path.at(i+1);
			if(flow_.edgeExists(s, d)){ // same direction - add capacity
				flow_.setEdgeWeight(s, d, flow_.getEdgeWeight(s, d) + cap);
			}
			else{ // opposite direction - subtract capcity
				flow_.setEdgeWeight(d, s, flow_.getEdgeWeight(d, s) - cap);
			}
			// subtract residual forward
			residual_.setEdgeWeight(s, d, residual_.getEdgeWeight(s, d) - cap);
			// add residual reverse
			residual_.setEdgeWeight(d, s, residual_.getEdgeWeight(d, s) + cap);
		}
	}
	int max = 0;
	for(Vertex v : flow_.getAdjacent(source_)){
		max += flow_.getEdgeWeight(source_, v);
	}
	maxFlow_ = max;
	return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}