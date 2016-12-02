#include <algorithm>
#include <vector>
#include "dijkstra.h"

bool DijkstraState::operator<(const DijkstraState& other) const
{
	return distance > other.distance;
}

Dijkstra::Dijkstra(const Graph* graph, const vector<double>* arc_lengths)
: _graph(graph), _arc_lengths(arc_lengths), _distances(graph->NumNodes(), std::numeric_limits<double>::infinity()), _parentsArc(graph->NumNodes(), -1), _arcsPath()
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
	std::vector<int> target(targets);
	std::priority_queue<DijkstraState> queue;

	if (target.size() == 0)
	{
		for (int i = 0;i < _graph->NumNodes();++i)
			target.push_back(i);
	}

	_reachedNode.clear();
	_distances.clear();
	_parentsArc.clear();
	_distances.resize(_graph->NumNodes(), std::numeric_limits<double>::infinity());
	_parentsArc.resize(_graph->NumNodes(), -1);

	_distances[source] = 0;
	_reachedNode.push_back(source);

	while (!target.empty() || !queue.empty())
	{
		Travel(source, queue, target);

		DijkstraState nextSource;

		do {
			if (queue.empty())
				return;
			nextSource = queue.top();
			queue.pop();
		} while(std::find(_reachedNode.begin(), _reachedNode.end(), nextSource.node) != _reachedNode.end());
		_reachedNode.push_back(source);

		source = nextSource.node;
	}
}

void Dijkstra::Travel(int source, std::priority_queue<DijkstraState>& queue,
						vector<int>& targets)
{
	vector<int> outArc = _graph->OutgoingArcs(source);

	// Erase Target if find
	std::vector<int>::iterator it = std::find(targets.begin(), targets.end(), source);
	if (it != targets.end())
		targets.erase(it);

	for (auto it : outArc)
	{
		int next = _graph->Head(it);

		if (_distances[source] + (*_arc_lengths)[it] < _distances[next])
		{
			_parentsArc[next] = it;
			_distances[next] = _distances[source] + (*_arc_lengths)[it];
			queue.push({next, _distances[next]});
		}
	}
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
	int i = node;
	vector<int> ret(0, 0);

	if (_parentsArc[i] == -1)
		return ret;

	do {
		ret.push_back(_parentsArc[i]);
		i = _graph->Tail(_parentsArc[i]);
	} while(_parentsArc[i] != -1 && i != node);

	std::reverse(ret.begin(), ret.end());
	return ret;
}
