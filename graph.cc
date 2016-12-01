#include "graph.h"

Graph::Graph()
{
	this->outgoing = std::vector<std::vector<int> >();
	this->incoming = std::vector<std::vector<int> >();
	this->arcs = std::vector<std::pair<int, int> >();
}

int Graph::AddArc(int from, int to)
{
	if (from > this->outgoing.size())
	{
		this->AddNode(from);
	}
	if (to > this->incoming.size())
	{
		this->AddNode(to);
	}

	this->arcs.push_back(std::pair<int, int>(from, to));

	int index = this->arcs.size() - 1;
	this->outgoing[from].push_back(index);
	this->incoming[to].push_back(index);

	return (index);
}

// Optional: nodes are automatically added upon AddArc().
void Graph::AddNode(int node)
{
	for (int i = this->outgoing.size(); i <= node; ++i)
	{
		this->outgoing.push_back(std::vector<int>());
		this->incoming.push_back(std::vector<int>());
	}
}

int Graph::NumNodes() const
{
	return this->outgoing.size();
}

int Graph::NumArcs() const
{
	return this->arcs.size();
}

int Graph::Tail(int arc) const
{
	return (this->arcs[arc].first);
}

int Graph::Head(int arc) const
{
	return (this->arcs[arc].second);
}

const std::vector<int>& Graph::OutgoingArcs(int from) const
{
	return this->outgoing[from];
}

const std::vector<int>& Graph::IncomingArcs(int to) const
{
	return this->incoming[to];
}
