#ifndef BIDI_DIJKSTRA_H_
#define BIDI_DIJKSTRA_H_

# include "dijkstra.h"

// Similar, but does bi-directional searches
// internally (faster).
class BidirectionalDijkstra {
private:

  const Graph& graph_;
  const vector<double>& arc_lengths_;

  int conv_point_;

  vector<double> distance_from_;
  vector<double> distance_to_;

  vector<int> parent_arc_;
  vector<int> child_arc_;
  vector<int> reached_nodes_from_;
  vector<int> reached_nodes_to_;
  vector<int> indexed_nodes_;

  priority_queue<DijkstraState> pq_to_;
  priority_queue<DijkstraState> pq_from_;
  priority_queue<DijkstraState> conv_points_;

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

#endif  // BIDI_DIJKSTRA_H_
