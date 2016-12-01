#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

struct DijkstraState {
  int node;
  double distance;
  bool operator<(const DijkstraState& other) const;
};

class Dijkstra {
 public:
  // "graph" and "arc_lengths" aren't copied.
  Dijkstra(const Graph* graph, const vector<double>* arc_lengths);

  void Run(int source);

  // "Read" API, after a dijkstra run has completed.
  const vector<double>& Distances() const;  // Infinity if not reached.
  const vector<int>& ParentArcs() const;    // -1 if not reached.
  vector<int> ArcPathFromSourceTo(int node) const;

 private:
  const Graph& graph_;
  const vector<double>& arc_lengths_;
  vector<double> distance_;
  vector<int> parent_arc_;
  priority_queue<DijkstraState> pq_;
};

#endif /* end of include guard: DIJKSTRA_H_ */
