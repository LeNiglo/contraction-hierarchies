#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

# define _DEBUG false

#include <limits>  // For std::numeric_limits<double>::infinity().
#include <queue>   // For std::priority_queue<>
#include <vector>

#include "base.h"
#include "graph.h"

using std::priority_queue;
using std::vector;

string NodePathOfArcPath(const Graph& graph, const vector<int>& arc_path, int src);

constexpr double kInfinity = std::numeric_limits<double>::infinity();

struct DijkstraState {
  int node;
  double distance;

  // So that we can do std::priority_queue<DijkstraState>. Beware the ordering!
  bool operator<(const DijkstraState& other) const {
    // Reverse the order because of priority_queue<> pop order.
    if (distance != other.distance) return distance > other.distance;
    // Break ties by node id.
    return node > other.node;
  }
};

// This class helps to run several Dijkstra computation serially (it it NOT
// thread safe) efficiently: by sharing some temporary data structures,
// only O(num edges explored) time is used by each Dijkstra computation,
// even if it's sparse (i.e. num edges explored <<< num nodes).
class Dijkstra {
 public:
  // The given graph and arc lengths won't be copied, and must remain live for
  // the lifetime of this class.
  Dijkstra(const Graph* graph, const vector<double>* arc_lengths);

  const Graph& GetGraph() const { return graph_; }

  // Main Dijkstra call: run a single-source search from source "source",
  // and stop when all the targets are reached.
  // If "targets" is empty, run until exhaustion (i.e. until all reachable
  // nodes are explored).
  void RunUntilAllTargetsAreReached(int source, const vector<int>& targets);

  // Returns the set of all nodes reached by the last run.
  const vector<int>& ReachedNodes() const { return reached_nodes_; }

  // Element #i is the distance of node #i from the source, in the last run,
  // which is infinity if node #i wasn't reached.
  const vector<double>& Distances() const { return distance_; }

  // Element #i is the arc that arrives at node #i in the shortest
  // path from the source, or -1 if that node wasn't reached. Also -1 if the
  // node is the source.
  const vector<int>& ParentArcs() const { return parent_arc_; }

  // Returns the full shortest path (a sequence of arcs) from the source of
  // the last run to "node", assuming that "node" was reached.
  vector<int> ArcPathFromSourceTo(int node) const;

 private:
  const Graph& graph_;
  const vector<double>& arc_lengths_;
  vector<bool> is_target_;
  vector<double> distance_;
  vector<int> parent_arc_;
  vector<int> reached_nodes_;
  priority_queue<DijkstraState> pq_;
};


// Similar, but does bi-directional searches
// internally (faster).
class BidirectionalDijkstra {
private:

  const Graph& graph_;
  const vector<double>& arc_lengths_;

  int conv_point_;
  // std::vector<int> conv_points_;

  vector<double> distance_to_;
  vector<double> distance_from_;
  vector<int> parent_arc_;
  vector<int> child_arc_;
  vector<int> reached_nodes_from_;
  vector<int> reached_nodes_to_;
  priority_queue<DijkstraState> pq_to_;
  priority_queue<DijkstraState> pq_from_;
  priority_queue<DijkstraState> conv_points_;
  vector<int> indexed_nodes_;

public:
  BidirectionalDijkstra(const Graph* graph, const std::vector<double>* arc_lengths);

  // Returns the arc path from "from" to "to".
  // If from == to or if there is no path,
  // returns an empty path.
  std::vector<int> FindShortestPath(int from, int to);

  // TODO(you): fill the rest and implement the .cc.
  int stepFrom(int target);
  int stepTo(int target);
	std::vector<int> ArcPathFromSourceTo(int node);
	std::vector<int> ArcPathFromTargetTo(int node);
};

#endif  // DIJKSTRA_H_
