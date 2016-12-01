#include "graph.h"

Graph::Graph()
{

}

int Graph::AddArc(int from, int to)
{
	this->arcs.push_back(std::pair<int, int>(from, to));

	int index = this->arcs.size() - 1;
	this->outgoing[from].push_back(index);
	this->incoming[to].push_back(index);

	return (index);
}

// Optional: nodes are automatically added upon AddArc().
void Graph::AddNode(int node, const std::pair<double, double> pos)
{
	if (this->NumNodes() <= node)
	{
		this->outgoing.resize(node + 1);
		this->incoming.resize(node + 1);
		this->positions.resize(node + 1);
	}
	this->positions[node] = pos;
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

const std::vector<std::pair<double, double> >&	Graph::getPositions()
{
	return this->positions;
}

void						Graph::display(const double d)
{
	std::cout << NumNodes() << std::endl;
	std::cout << NumArcs() << std::endl;
	printf("%lf\n", d);
}
