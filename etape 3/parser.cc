#include "parser.h"

Parser::Parser(const std::string &filename)
{
	_duration = 0;
	this->_filename = filename;
	this->_points = std::map<std::pair<double, double>, int>();
	this->_index = std::map<std::pair<double, double>, int>();
}


bool			Parser::parseFile(std::vector<t_road>& roads)
{
	std::ifstream infile(this->_filename);
	if (!infile)
	{
		return (false);
	}

	std::string line;
	while (std::getline(infile, line))
	{
		t_road					road;
		std::istringstream		iss(line);
		std::string 			token;
		std::string				tmp_token;
		int						i = 0;

		while (std::getline(iss, token, ','))
		{
			switch(i)
			{
				case 0: //name;
				road._name = token; break;
				case 1: //priority;
				road._priority = std::atoi(token.c_str()); break;
				case 2: //max_speed;
				road._maxSpeed = std::atoi(token.c_str()); break;
				case 3: //num_lanes;
				road._numLanes = std::atoi(token.c_str()); break;
				case 4: //oneway;
				road._oneway = (std::atoi(token.c_str()) == 1); break;
				case 5: //numpoints;
				break;
				default:
				if (i % 2 == 0)
				{
					tmp_token = token;
				}
				else
				{
					std::pair<double, double>	pair(std::atof(tmp_token.c_str()), std::atof(token.c_str()));
					road._points.push_back(pair);
					++_points[pair];
				}
				break;
			};
			++i;
		}
		roads.push_back(road);
	}
	return (true);
}

void			Parser::createNode(const std::pair<double, double> &pair, Graph &graph)
{
	graph.AddNode(_nodeIndex++, pair);
	_index[pair] = _nodeIndex;
}

double			Parser::parseRoads(std::vector<t_road>& roads, Graph &graph)
{
	for (auto& road : roads)
	{
		for (int i = 0, roadPointsSize = road._points.size(); i < roadPointsSize; ++i)
		{
			std::pair<double, double> pair = road._points[i];

			if (i == 0 || _points[pair] > 1 || i == roadPointsSize - 1)
			{
				if (_index[pair] == 0)
				{
					createNode(pair, graph);
				}

				if (i != 0)
				{
					bool lastNodePassed = false;
					for (int j = 0; j < roadPointsSize; ++j)
					{
						if (road._points[j].first == _lastNode.first && road._points[j].second == _lastNode.second)
						{
							lastNodePassed = true;
						}

						if (lastNodePassed || true)
						{
							_duration += distanceEarth(
								road._points[j].first,
								road._points[j].second,
								_lastNode.first,
								_lastNode.second
							) / (double) road._maxSpeed * 3600.0;

							fprintf(stderr, "Calc duration on %s @ %dkm/h: %lf km in %lf s\n", road._name.c_str(), road._maxSpeed, distanceEarth(
								road._points[j].first,
								road._points[j].second,
								_lastNode.first,
								_lastNode.second
							), distanceEarth(
								road._points[j].first,
								road._points[j].second,
								_lastNode.first,
								_lastNode.second
							) / (double) road._maxSpeed * 3600.0);
						}


					}

					graph.AddArc(_index[_lastNode] - 1, _index[pair] - 1);
					if (!road._oneway)
					{
						graph.AddArc(_index[pair] - 1, _index[_lastNode] - 1);
					}
				}

				_lastNode = pair;
			}
		}
	}
	return (_duration);
}
