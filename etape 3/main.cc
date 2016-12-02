#include "parse_csv.h"
#include "dijkstra.h"

int								findNodeAt(RoadData &data, const std::pair<double, double> &pos)
{
	if (data.latlng_to_node.count(pos) > 0)
	return data.latlng_to_node[pos];
	else
	return -1;
}

std::pair<std::pair<double, double>, std::pair<double, double> >	parseLine(std::string &line)
{
	double lat1, lon1, lat2, lon2;
	char skip;
	std::istringstream iss(line);

	iss >> lat1 >> skip >> lon1 >> skip >> skip >> lat2 >> skip >> lon2;
	return (std::pair<
		std::pair<double, double>,
		std::pair<double, double>
		>(
			std::pair<double, double>(lat1, lon1),
			std::pair<double, double>(lat2, lon2)
		)
	);
}

int main(int argc, char** argv) {
	if (argc != 2)
	{
		return (EXIT_FAILURE);
	}
	RoadData data = ParseCsvFile(argv[1]);

	std::string line;
	while (std::cin >> line){
		std::vector<int> nodes;
		int node, node2;

		std::pair<std::pair<double, double>, std::pair<double, double> > query = parseLine(line);
		fprintf(stderr, "[%lf , %lf]  =>  [%lf , %lf]\n", query.first.first, query.first.second, query.second.first, query.second.second);
		node = findNodeAt(data, query.first);
		node2 = findNodeAt(data, query.second);

		fprintf(stderr, "%d // %d\n", node, node2);
		if (node != -1 && node2 != -1)
		{
			nodes.push_back(findNodeAt(data, query.second));
			Dijkstra dijkstra(&data.graph, &data.arc_durations);
			dijkstra.RunUntilAllTargetsAreReached(node, nodes);

			// if ((distance = dijkstra.getDistance()) == infinity) => "NO PATH"

		}
		else
		{
			std::cout << "INVALID" << std::endl;
		}

	}

	return (EXIT_SUCCESS);
}
