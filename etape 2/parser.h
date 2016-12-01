#ifndef PARSER_H_
# define PARSER_H_

# include <iostream>
# include <fstream>
# include <memory>
# include <sstream>
# include <string>
# include <cstdlib>

# include "graph.h"
# include "road.h"

class Parser
{
private:
	std::string									_filename;
	std::map<std::pair<double, double>, int>	_points;
	std::map<std::pair<double, double>, int>	_index;

	std::pair<double, double> 					_lastNode;
	int											_nodeIndex;
	double										_duration;

public:
	Parser(const std::string &filename);
	bool			parseFile(std::vector<t_road>& roads);
	void			createNode(const std::pair<double, double> &pair, Graph &graph);
	double			parseRoads(std::vector<t_road>& roads, Graph &graph);
};

double distanceEarth(double, double, double, double);

#endif /* PARSER_H_ */
