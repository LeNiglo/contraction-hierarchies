#ifndef PARSER_H_
# define PARSER_H_

# include <iostream>
# include <fstream>
# include <memory>
# include <sstream>
# include <string>
# include <cstdlib>

# include "graph.h"
# include "road.hpp"
# include "calclatlong/haversine.h"

class Parser
{
private:
	std::string									_filename;
	std::map<std::pair<double, double>, int>	_points;

	std::pair<double, double> 					_lastNode;
	int											_nodeIndex;

public:
	Parser(const std::string &filename);
	bool			parseFile(std::vector<std::shared_ptr<Road> >& roads, Graph &graph);
	void			createNode(const std::pair<double, double> &pair, const std::shared_ptr<Road> &road, Graph &graph, const char &beginend);
	int				getIndexOfLastNode(Graph &graph);


	bool			parseRoads(std::vector<std::shared_ptr<Road> >& roads, Graph &graph);
};

#endif /* PARSER_H_ */
