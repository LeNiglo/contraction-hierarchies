#include "dijkstra.h"

bool DijkstraState::operator<(const DijkstraState& other) const
{
	
}

Dijkstra::Dijkstra(const Graph* graph, const vector<double>* arc_lengths)
: _graph(graph), _arc_lengths(arc_lengths)
{
}

const Graph& Dijkstra::GetGraph() const
{
	return *_graph;
}

// Main Dijkstra call: run a single-source search from source "source",
// and stop when all the targets are reached.
// If "targets" is empty, run until exhaustion (i.e. until all reachable
// nodes are explored).
void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int>& targets)
{
}

// Returns the set of all nodes reached by the last run.
const vector<int>& Dijkstra::ReachedNodes() const
{
	return _reachedNode;
}


// Element #i is the distance of node #i from the source, in the last run,
// which is infinity if node #i wasn't reached.
const vector<double>& Dijkstra::Distances() const
{
	return _distances;
}

// Element #i is the arc that arrives at node #i in the shortest
// path from the source, or -1 if that node wasn't reached. Also -1 if the
// node is the source.
const vector<int>& Dijkstra::ParentArcs() const
{
	return _parentsArc;
}

// Returns the full shortest path (a sequence of arcs) from the source of
// the last run to "node", assuming that "node" was reached.
vector<int> Dijkstra::ArcPathFromSourceTo(int node) const
{
	return _arcsPath;
}
