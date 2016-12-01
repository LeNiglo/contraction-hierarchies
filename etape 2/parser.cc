#include "parser.h"

Parser::Parser(const std::string &filename)
{
	_duration = 0;
	this->_filename = filename;
	this->_points = std::map<std::pair<double, double>, int>();
	this->_index = std::map<std::pair<double, double>, int>();
}


double			Parser::parseFile(std::vector<std::shared_ptr<Road> >& roads, Graph &graph)
{
	std::ifstream infile(this->_filename);
	if (!infile)
	{
		return false;
	}
	
	std::string line;
	while (std::getline(infile, line))
	{
		std::shared_ptr<Road>	road(new Road());
		std::istringstream		iss(line);
		std::string 			token;
		std::string				tmp_token;
		int						i = 0;
		int						numpoints;
		
		while (std::getline(iss, token, ','))
		{
			switch(i)
			{
				case 0:
				//name;
				road->setName(token);
				break;
				case 1:
				//priority;
				road->setPriority(std::atoi(token.c_str()));
				break;
				case 2:
				//max_speed;
				road->setMaxSpeed(std::atoi(token.c_str()));
				break;
				case 3:
				//num_lanes;
				road->setNumLanes(std::atoi(token.c_str()));
				break;
				case 4:
				//oneway;
				road->setOneway(std::atoi(token.c_str()) == 1);
				break;
				case 5:
				//numpoints;
				numpoints = std::atoi(token.c_str());
				break;
				default:
				if (i % 2 == 0)
				{
					tmp_token = token;
				}
				else
				{
					std::pair<double, double>	pair(std::atof(tmp_token.c_str()), std::atof(token.c_str()));
					road->AddPoints(pair);
					++_points[pair];
					
					if (_points[pair] < 2)
					{
						if (i == 7)
						{
							createNode(pair, road, graph, 0);
						}
						else if (i == 5 + numpoints * 2)
						{
							createNode(pair, road, graph, 1);
						}
					}
					else if (_points[pair] == 2)
					{
						createNode(pair, road, graph, -1);
					}
					
					
					if (_points[pair] >= 2 && i != 7)
					{
						bool		lastNodePassed = false;
						
						int idx = _index[_lastNode];
						
						for (int i = 0; i < road->getPoints().size(); i++)
						{
							if (lastNodePassed || (road->getPoints()[i].first == _lastNode.first && road->getPoints()[i].second == _lastNode.second))
							{
								lastNodePassed = true;
								Haversine haversine(road->getPoints()[i].first, road->getPoints()[i].second, _lastNode.first, _lastNode.second);
								_duration += haversine.DistanceKm() / (double) road->getMaxSpeed() * 3600.0;
							}
						}
						
						graph.AddArc(idx, _nodeIndex - 1);
						if (!road->getOneway())
						{
							graph.AddArc(_nodeIndex - 1, idx);
						}
						
					}
					
					if (_points[pair] >= 2)
					{
						_lastNode = pair;
					}
					
				}
				break;
			};
			++i;
		}
	}
	return _duration;
}

void			Parser::createNode(const std::pair<double, double> &pair, const std::shared_ptr<Road> &road, Graph &graph, const char &beginend)
{
	_index[pair] = _nodeIndex;
	graph.AddNode(_nodeIndex++, pair);
	
	if (beginend == 0 || beginend == 1)
	{
		_points[pair] = 102;
	}
}

bool			Parser::parseRoads(std::vector<std::shared_ptr<Road> >& roads, Graph &graph)
{
	return true;
}
