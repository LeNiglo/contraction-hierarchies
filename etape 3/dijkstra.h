#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <limits>  // For std::numeric_limits<double>::infinity().
#include <queue>   // For std::priority_queue<>
#include <vector>

#include "base.h"
#include "graph.h"

using std::priority_queue;
using std::vector;

struct DijkstraState {
	int node;
	double distance;

	// So that we can do std::priority_queue<DijkstraState>. Beware the ordering!
	bool operator<(const DijkstraState& other) const;
};

// This class helps to run several Dijkstra computation serially (it it NOT
// thread safe) efficiently: by sharing some temporary data structures,
// only O(num edges explored) time is used by each Dijkstra computation,
// even if it's sparse (i.e. num edges explored <<< num nodes).
class Dijkstra {
private:
	const Graph* _graph;
	const vector<double>* _arc_lengths;

	vector<int> _reachedNode;
	vector<double> _distances;
	vector<int> _parentsArc;
	vector<int> _arcsPath;

public:
	// The given graph and arc lengths won't be copied, and must remain live for
	// the lifetime of this class.
	Dijkstra(const Graph* graph, const vector<double>* arc_lengths);

	void Travel(int source, std::priority_queue<DijkstraState>& queue, vector<int>& targets);

	const Graph& GetGraph() const;

	// Main Dijkstra call: run a single-source search from source "source",
	// and stop when all the targets are reached.
	// If "targets" is empty, run until exhaustion (i.e. until all reachable
	// nodes are explored).
	void RunUntilAllTargetsAreReached(int source, const vector<int>& targets);

	// Returns the set of all nodes reached by the last run.
	const vector<int>& ReachedNodes() const;

	// Element #i is the distance of node #i from the source, in the last run,
	// which is infinity if node #i wasn't reached.
	const vector<double>& Distances() const;

	// Element #i is the arc that arrives at node #i in the shortest
	// path from the source, or -1 if that node wasn't reached. Also -1 if the
	// node is the source.
	const vector<int>& ParentArcs() const;

	// Returns the full shortest path (a sequence of arcs) from the source of
	// the last run to "node", assuming that "node" was reached.
	vector<int> ArcPathFromSourceTo(int node) const;
};

#endif  // DIJKSTRA_H_
