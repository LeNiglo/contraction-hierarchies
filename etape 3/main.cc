#include "parser.h"

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage : ./server data.csv" << std::endl;
		return (EXIT_FAILURE);
	}

	Parser		parser(av[1]);
	std::vector<t_road> roads;
	Graph		graph;

	parser.parseFile(roads);
	double d = parser.parseRoads(roads, graph);
	graph.display(d);
	return (EXIT_SUCCESS);
}
