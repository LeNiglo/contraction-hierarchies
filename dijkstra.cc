#include "dijkstra.h"

#include <algorithm>
#include <iostream>

#include "base.h"
#include "dijkstra.h"

using namespace std;

Dijkstra::Dijkstra(const Graph* graph, const vector<double>* arc_lengths)
  : graph_(*graph), arc_lengths_(*arc_lengths),
    is_target_(graph_.NumNodes(), false),
    distance_(graph_.NumNodes(), kInfinity),
    parent_arc_(graph_.NumNodes(), -1) {}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int>& targets) {
  CHECK(pq_.empty());

  // Clean up the last Dijkstra run, sparsely.
  for (const int node : reached_nodes_) {
    distance_[node] = kInfinity;
    parent_arc_[node] = -1;
  }
  reached_nodes_.clear();

  // Support incremental growth of the graph across the lifetime of the
  // Dijkstra. This is especially useful for contraction hierarchies.
  // NOTE: We don't support graph shrinkage, but we could.
  const int num_nodes = graph_.NumNodes();
  CHECK_GE(num_nodes, is_target_.size())
      << "The graph shrunk since the construction of the Dijkstra object!";
  if (num_nodes > is_target_.size()) {
    is_target_.resize(num_nodes, false);
    distance_.resize(num_nodes, kInfinity);
    parent_arc_.resize(num_nodes, -1);
  }

  // Basic initialization.
  distance_[source] = 0;
  reached_nodes_.push_back(source);
  pq_.push({source, 0, false});

  // Collect the targets.
  int num_targets_left = 0;
  if (targets.empty()) {
    num_targets_left = num_nodes;
  } else {
    for (const int t : targets) {
      if (!is_target_[t]) {
        ++num_targets_left;
        is_target_[t] = true;
      }
    }
  }

  // Run the Dijkstra exploration.
  int num_settled = 0;
  while (!pq_.empty()) {
    const int node = pq_.top().node;
    const double distance = pq_.top().distance;
    pq_.pop();
    ++num_settled;
    if (distance > distance_[node]) continue;
    if (is_target_[node] && --num_targets_left == 0) break;
    for (const int arc : graph_.OutgoingArcs(node)) {
      const int head = graph_.Head(arc);
      const double d = distance_[node] + arc_lengths_[arc];
      if (d >= distance_[head]) continue;
      if (distance_[head] == kInfinity) reached_nodes_.push_back(head);
      distance_[head] = d;
      parent_arc_[head] = arc;
      pq_.push({head, d, false});
    }
  }

  // Reset the 'temporary' data structures to their initial state.
  for (const int t : targets) is_target_[t] = false;
  while (!pq_.empty()) pq_.pop();  // We should probably just reinitialize it.
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {
  vector<int> arcs;
  for (;;) {
    const int a = parent_arc_[node];
    if (a < 0) break;
    arcs.push_back(a);
    node = graph_.Tail(a);
  }
  std::reverse(arcs.begin(), arcs.end());
  return arcs;
}
