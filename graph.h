#ifndef GRAPH_H_
# define GRAPH_H_

# include <vector>
# include <utility>

// In this graph, nodes are integers in 0..num_nodes-1, and arcs are also
// integers in 0..num_arcs-1 (in the order in which they're added)
// Thus, when you AddArc(4, 128), you implicitly declare the existence of
// at least 129 nodes (0..128).
class Graph {
private:
	std::vector<std::vector<int> >		outgoing;
	std::vector<std::vector<int> >		incoming;
	std::vector<std::pair<int, int> >	arcs;

public:
	Graph();
	// Returns the arc index.
	int		AddArc(int from, int to);

	// Optional: nodes are automatically added upon AddArc().
	void	AddNode(int node);

	int		NumNodes() const;
	int		NumArcs() const;

	// Gets the tail ("from") and head ("to") of an arc.
	int		Tail(int arc) const;
	int		Head(int arc) const;

	// Returns a list of all the arc indices whose Tail is "from".
	const std::vector<int>&		OutgoingArcs(int from) const;

	// Returns a list of all the arc indices whose Head is "to".
	const std::vector<int>&		IncomingArcs(int to) const;
};

#endif /* GRAPH_H_ */
