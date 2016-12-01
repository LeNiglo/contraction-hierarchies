#include "parser.h"

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage : ./server data.csv" << std::endl;
		return (EXIT_FAILURE);
	}
	
	Parser		parser(av[1]);
	std::vector<std::shared_ptr<Road> >	roads;
	Graph		graph;
	
	Haversine haversine(33, 1, -45,-9);
	std::cout << haversine.DistanceKm() * 1000 << std::endl;
	
	int d = parser.parseFile(roads, graph);
	parser.parseRoads(roads, graph);
	graph.display(d);
	return (EXIT_SUCCESS);
}
