#include "dijkstra.h"

BidirectionalDijkstra::BidirectionalDijkstra(const Graph* graph, const vector<double>* arc_lengths)
  : graph_(*graph), arc_lengths_(*arc_lengths),
    distance_from_(graph_.NumNodes(), kInfinity),
    distance_to_(graph_.NumNodes(), kInfinity),
    parent_arc_(graph_.NumNodes(), -1),
    child_arc_(graph_.NumNodes(), -1) {}


std::vector<int> BidirectionalDijkstra::FindShortestPath(int from, int to)
{
  CHECK(pq_from_.empty());
  CHECK(pq_to_.empty());

  std::cout << "FindShortestPath(" << from << ", " << to << ");" << std::endl;

  // Clean up the last Dijkstra run, sparsely.
  for (const int node : reached_nodes_from_) {
    distance_from_[node] = kInfinity;
    parent_arc_[node] = -1;
  }
  reached_nodes_from_.clear();

  for (const int node : reached_nodes_to_) {
    distance_to_[node] = kInfinity;
    child_arc_[node] = -1;
  }
  reached_nodes_to_.clear();

  // Support incremental growth of the graph across the lifetime of the
  // Dijkstra. This is especially useful for contraction hierarchies.
  // NOTE: We don't support graph shrinkage, but we could.
  const int num_nodes = graph_.NumNodes();
  CHECK_GE(num_nodes, 1)
      << "The graph shrunk since the construction of the Dijkstra object!";
  if (num_nodes > 1) {
    distance_from_.resize(num_nodes, kInfinity);
    distance_to_.resize(num_nodes, kInfinity);
    parent_arc_.resize(num_nodes, -1);
    child_arc_.resize(num_nodes, -1);
  }

  // Basic initialization.
  distance_from_[from] = 0;
  distance_to_[to] = 0;
  reached_nodes_from_.push_back(from);
  reached_nodes_to_.push_back(to);
  pq_from_.push({from, 0});
  pq_to_.push({to, 0});

  int stepFromContinue = 1;
  int stepToContinue = 1;

  std::cout << "run exploration" << std::endl;
  // Run the Dijkstra exploration.
  while (
    stepFromContinue == 1 && stepToContinue == 1
  ) {
      stepFromContinue = this->stepFrom(to);
      stepToContinue = this->stepTo(from);
  }
  std::cout << "finished exploration" << std::endl;

  // Reset the 'temporary' data structures to their initial state.
  while (!pq_from_.empty()) pq_from_.pop();  // We should probably just reinitialize it.
  while (!pq_to_.empty()) pq_to_.pop();  // We should probably just reinitialize it.

  if (stepFromContinue == 0)
  {
    std::cout << "Break From 'FROM'" << std::endl;
    return this->ArcPathFromSourceTo(to);
  }
  if (stepToContinue == 0)
  {
    std::cout << "Break From 'TO'" << std::endl;
    return this->ArcPathFromTargetTo(from);
  }
  if (stepFromContinue == -1 && stepToContinue == -1)
  {
    std::cout << "Returns EMPTY PATHS" << std::endl;
    return std::vector<int>();
  }
  std::cout << "???? => " << stepFromContinue << ", " << stepToContinue << std::endl;
  return std::vector<int>();
}


/**
*     1 => SKIP but continue loop
*     0 => FOUND the target
*     -1 => PQ is empty
*/
int     BidirectionalDijkstra::stepFrom(int target)
{
  if (!pq_from_.empty())
  {
    const int node = pq_from_.top().node;
    const double distance = pq_from_.top().distance;
    pq_from_.pop();
    if (graph_.OutgoingArcs(node).size() == 0) return -1;
    if (distance > distance_from_[node]) return 1;
    if (target == node) return 0;
    for (const int arc : graph_.OutgoingArcs(node)) {
      const int head = graph_.Head(arc);
      const double d = distance_from_[node] + arc_lengths_[arc];
      if (d >= distance_from_[head]) return 1;
      if (distance_from_[head] == kInfinity) reached_nodes_from_.push_back(head);
      distance_from_[head] = d;
      std::cout << "distance_from_[" << head << "] = " << d << std::endl;
      parent_arc_[head] = arc;
      pq_from_.push({head, d});
    }
    return 1;
  }
  return -1;
}

int     BidirectionalDijkstra::stepTo(int target)
{
  if (!pq_to_.empty())
  {
    const int node = pq_to_.top().node;
    const double distance = pq_to_.top().distance;
    pq_to_.pop();
    if (graph_.IncomingArcs(node).size() == 0) return -1;
    if (distance > distance_to_[node]) return 1;
    if (target == node) return 0;
    for (const int arc : graph_.IncomingArcs(node)) {
      const int tail = graph_.Tail(arc);
      const double d = distance_to_[node] + arc_lengths_[arc];
      if (d >= distance_to_[tail]) return 1;
      if (distance_to_[tail] == kInfinity) reached_nodes_to_.push_back(tail);
      distance_to_[tail] = d;
      std::cout << "distance_to_[" << tail << "] = " << d << std::endl;
      child_arc_[tail] = arc;
      pq_to_.push({tail, d});
    }
    return 1;
  }
  return -1;
}

vector<int> BidirectionalDijkstra::ArcPathFromSourceTo(int node) const
{
  std::cout << "ArcPathFromSourceTo ... \t";
  vector<int> arcs;
  for (;;) {
    const int a = parent_arc_[node];
    if (a < 0) break;
    arcs.push_back(a);
    node = graph_.Tail(a);
  }
  std::reverse(arcs.begin(), arcs.end());
  std::cout << "Done." << std::endl;
  return arcs;
}

vector<int> BidirectionalDijkstra::ArcPathFromTargetTo(int node) const
{
  std::cout << "ArcPathFromSourceTo ... \t";
  vector<int> arcs;
  for (;;) {
    const int a = child_arc_[node];
    if (a < 0) break;
    arcs.push_back(a);
    node = graph_.Head(a);
  }
  // std::reverse(arcs.begin(), arcs.end());
  std::cout << "Done." << std::endl;
  return arcs;
}
