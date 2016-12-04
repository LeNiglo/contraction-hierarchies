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

void processLine(RoadData &data, std::string &line)
{
	double duration = 0;
	std::pair<std::pair<double, double>, std::pair<double, double> > query = parseLine(line);
	int node = findNodeAt(data, query.first);
	int node2 = findNodeAt(data, query.second);

	if (node != -1 && node2 != -1)
	{
		BidirectionalDijkstra dijkstra(&data.graph, &data.arc_durations);
		std::vector<int> path = dijkstra.FindShortestPath(node, node2);

		if (path.empty())
		{
			std::cout << "NO PATH" << std::endl;
		}
		else
		{
			for (auto& it: path)
			{
				duration += data.arc_durations[it];
			}
			std::cout << duration;
			for (auto& it: path)
			{
					std::cout << " " << data.arc_durations[it];
			}
			std::cout << std::endl;
		}

	}
	else
	{
		std::cout << "INVALID" << std::endl;
	}
}


int main(int argc, char** argv) {
	if (argc != 2)
	{
		return (EXIT_FAILURE);
	}
	RoadData data = ParseCsvFile(argv[1]);

	std::string line;
	while (std::getline(cin, line)){
		processLine(data, line);
		line = "";
	}

	return (EXIT_SUCCESS);
}
