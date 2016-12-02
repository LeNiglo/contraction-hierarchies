#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

// In this graph, nodes are integers in 0..num_nodes-1, and arcs are also
// integers in 0..num_arcs-1.
// Thus, when you AddArc(4, 128), you implicitly declare the existence of
// at least 129 nodes (0..128).
class Graph {
 public:
  // Return the arc index, which will be the number of times AddArc()
  // has been called before.
  int AddArc(int from, int to);

  // Optional: nodes are automatically added upon AddArc().
  void AddNode(int node);

  int NumNodes() const { return outgoing_arcs_.size(); }
  int NumArcs() const { return tail_.size(); }

  // Gets the tail ("from") and head ("to") of an arc.
  int Tail(int arc) const { return tail_[arc]; }
  int Head(int arc) const { return head_[arc]; }

  // Returns a list of all the arc indices whose Tail is "from".
  const vector<int>& OutgoingArcs(int from) const {
    return outgoing_arcs_[from];
  }
  // Returns a list of all the arc indices whose Head is "to".
  const vector<int>& IncomingArcs(int to) const {
    return incoming_arcs_[to];
  }

 private:
  vector<vector<int>> outgoing_arcs_;
  vector<vector<int>> incoming_arcs_;
  vector<int> tail_;
  vector<int> head_;
};

#endif  // GRAPH_H_
