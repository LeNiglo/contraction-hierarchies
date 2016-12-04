#include "dijkstra.h"

BidirectionalDijkstra::BidirectionalDijkstra(const Graph* graph, const vector<double>* arc_lengths)
  : graph_(*graph), arc_lengths_(*arc_lengths),
    indexed_nodes_(graph_.NumNodes(), 0),
    distance_from_(graph_.NumNodes(), kInfinity),
    distance_to_(graph_.NumNodes(), kInfinity),
    parent_arc_(graph_.NumNodes(), -1),
    child_arc_(graph_.NumNodes(), -1),
    conv_point_(-1) {}


std::vector<int> BidirectionalDijkstra::FindShortestPath(int from, int to)
{
  CHECK(pq_from_.empty());
  CHECK(pq_to_.empty());

  if (from == to)
  return std::vector<int>();

  conv_point_ = -1;

  if (_DEBUG)
    std::cerr << "\n  =>  FindShortestPath(" << from << ", " << to << ");\n" << std::endl;

  // Clean up the last Dijkstra run, sparsely.
  for (const int node : reached_nodes_from_) {
    indexed_nodes_[node] = 0;
    distance_from_[node] = kInfinity;
    parent_arc_[node] = -1;
  }
  reached_nodes_from_.clear();

  for (const int node : reached_nodes_to_) {
    indexed_nodes_[node] = 0;
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
    indexed_nodes_.resize(num_nodes, 0);
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

  indexed_nodes_[from] |= 0x01;
  indexed_nodes_[to] |= 0x02;

  if (_DEBUG)
    std::cerr << "run exploration" << std::endl;
  // Run the Dijkstra exploration.
  while (
    stepFromContinue == 1 && stepToContinue == 1
  ) {
      stepFromContinue = this->stepFrom(to);
      stepToContinue = this->stepTo(from);
	//   double min = kInfinity;
	//   for (auto it : this->conv_points_)
	//   {
	// 	  std::cerr << "Point : "<< it  << std::endl;
	// 	  std::cerr << "Distance : "<< this->distance_from_[it] + this->distance_to_[it]  << std::endl;
	// 	  std::cerr << "Min : "<< min  << std::endl;
	// 	  if (this->distance_from_[it] + this->distance_to_[it] < min)
	// 	  {
	// 		  min = this->distance_from_[it] + this->distance_to_[it];
	// 		  conv_point_ = it;
	// 	  }
	//   }
	//   if (this->conv_points_.size())
	//   {
	// 	  std::cerr << "Test: "<< this->pq_from_.top().node << std::endl;
	// 	  std::cerr << "Distance: "<< this->pq_from_.top().distance << std::endl;
	  //
	// 	  std::cerr << "Conv Point: "<< this->conv_point_  << std::endl;
	// 	  stepToContinue = 0;
	// 	  stepFromContinue = 0;
	//   }

      if (_DEBUG)
        std::cerr << "================================" << std::endl;
  }
  if (_DEBUG)
    std::cerr << "finished exploration" << std::endl;

  // Reset the 'temporary' data structures to their initial state.
  while (!pq_from_.empty()) pq_from_.pop();  // We should probably just reinitialize it.
  while (!pq_to_.empty()) pq_to_.pop();  // We should probably just reinitialize it.

  if (stepFromContinue == 0 || stepToContinue == 0)
  {
	this->conv_point_ = conv_points_.top().node;
	while (!conv_points_.empty()) conv_points_.pop();  // We should probably just reinitialize it.

    if (_DEBUG)
      std::cerr << "FOUND @ point #" << conv_point_ << " => " << stepFromContinue << ", " << stepToContinue << std::endl;

    std::vector<int> v1, v2;
    if (this->conv_point_ == to)
    {
      v1 = this->ArcPathFromSourceTo(this->conv_point_);
    }
    else if (this->conv_point_ == from)
    {
      v1 = this->ArcPathFromTargetTo(this->conv_point_);
    }
    else
    {
      v1 = this->ArcPathFromSourceTo(this->conv_point_);
      v2 = this->ArcPathFromTargetTo(this->conv_point_);
      if (_DEBUG)
      {
        std::cerr << NodePathOfArcPath(graph_, v1, to) << " // " << NodePathOfArcPath(graph_, v2, from) << std::endl;
      }
      v1.insert(v1.end(), v2.begin(), v2.end());

    }
    return v1;
  }
  while (!conv_points_.empty()) conv_points_.pop();  // We should probably just reinitialize it.

  if (_DEBUG)
    std::cerr << "???? => " << stepFromContinue << ", " << stepToContinue << std::endl;
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
    if (_DEBUG)
      std::cerr << "[FROM] node #" << node << ", looking for #" << target << std::endl;
    const double distance = pq_from_.top().distance;
    pq_from_.pop();
    if (graph_.OutgoingArcs(node).size() == 0) return -1;
    if (distance > distance_from_[node]) return 1;
	if (!conv_points_.empty() && distance > distance_from_[conv_points_.top().node]) return 0;
	if (target == node) return 0;
    for (const int arc : graph_.OutgoingArcs(node)) {
      const int head = graph_.Head(arc);
      const double d = distance_from_[node] + arc_lengths_[arc];
      if (_DEBUG)
        std::cerr << "distance_from_[" << head << "] = " << d << std::endl;
      if (indexed_nodes_[head] & 0x02) {
		  this->conv_points_.push({head, d + distance_to_[head]});
	  }
      if (d >= distance_from_[head]) continue;
      if (distance_from_[head] == kInfinity)
      {
        reached_nodes_from_.push_back(head);
        indexed_nodes_[head] |= 0x01;
      }
      distance_from_[head] = d;
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
    if (_DEBUG)
      std::cerr << "[TO] node #" << node << ", looking for #" << target << std::endl;
    const double distance = pq_to_.top().distance;
    pq_to_.pop();
    if (graph_.IncomingArcs(node).size() == 0) return -1;
    if (distance > distance_to_[node]) return 1;
	if (!conv_points_.empty() && distance > distance_to_[conv_points_.top().node]) return 0;
    if (target == node) return 0;
    for (const int arc : graph_.IncomingArcs(node)) {
      const int tail = graph_.Tail(arc);
      const double d = distance_to_[node] + arc_lengths_[arc];
      if (_DEBUG)
        std::cerr << "distance_to_[" << tail << "] = " << d << std::endl;
      if (indexed_nodes_[tail] & 0x01) {
		  this->conv_points_.push({tail, d + distance_from_[tail]});
	  }
      if (d >= distance_to_[tail]) continue;
      if (distance_to_[tail] == kInfinity)
      {
        reached_nodes_to_.push_back(tail);
        indexed_nodes_[tail] |= 0x02;
      }
      distance_to_[tail] = d;
      child_arc_[tail] = arc;
      pq_to_.push({tail, d});
    }

    return 1;
  }
  return -1;
}

vector<int> BidirectionalDijkstra::ArcPathFromSourceTo(int node)
{
  vector<int> arcs;
  for (;;) {
    const int a = parent_arc_[node];
    if (a < 0) break;
    if (_DEBUG)
    {
      // std::cerr << "parent_arc_[" << node << "] == " << a << std::endl;
    }
    arcs.push_back(a);
    node = graph_.Tail(a);
  }
  std::reverse(arcs.begin(), arcs.end());
  return arcs;
}

vector<int> BidirectionalDijkstra::ArcPathFromTargetTo(int node)
{
  vector<int> arcs;
  for (;;) {
    const int a = child_arc_[node];
    if (a < 0) break;
    if (_DEBUG)
    {
      // std::cerr << "child_arc_[" << node << "] == " << a << std::endl;
    }
    arcs.push_back(a);
    node = graph_.Head(a);
  }
  // std::reverse(arcs.begin(), arcs.end());
  return arcs;
}
