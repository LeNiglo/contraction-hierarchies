#ifndef ROAD_H_
# define ROAD_H_

typedef struct									s_road {
	std::string									_name;
	int											_priority;
	int											_maxSpeed;
	int											_numLanes;
	bool										_oneway;
	std::vector<std::pair<double, double> >		_points;
}												t_road;

#endif /* ROAD_H_ */
