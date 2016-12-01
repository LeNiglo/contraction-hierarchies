#include "parser.h"

Parser::Parser(const std::string &filename)
{
	_duration = 0;
	this->_filename = filename;
	this->_points = std::map<std::pair<double, double>, int>();
	this->_index = std::map<std::pair<double, double>, int>();
}


double			Parser::parseFile(std::vector<std::shared_ptr<Road> >& roads)
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
				}
				break;
			};
			++i;
		}
		roads.push_back(road);
	}
	return 0;
}

void			Parser::createNode(const std::pair<double, double> &pair, Graph &graph)
{
	graph.AddNode(_nodeIndex++, pair);
	_index[pair] = _nodeIndex;
}

bool			Parser::parseRoads(std::vector<std::shared_ptr<Road> >& roads, Graph &graph)
{
	double		distance;
	
	for (auto& road : roads)
	{
		for (int i = 0; i < road->getPoints().size(); ++i)
		{
			std::pair<double, double> pair = road->getPoints()[i];
			
			if (i == 0 || _points[pair] > 1 || i == road->getPoints().size() - 1)
			{
				if (_index[pair] == 0)
				{
					createNode(pair, graph);
				}
				
				if (i != 0)
				{
					graph.AddArc(_index[_lastNode] - 1, _index[pair] - 1);
					if (!road->getOneway())
					{
						graph.AddArc(_index[pair] - 1, _index[_lastNode] - 1);
					}
				}
				
				_lastNode = pair;
			}
			
		}
	}
	return (true);
}
/*

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

return true;
}
*/
