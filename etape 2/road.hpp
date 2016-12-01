#ifndef ROAD_H_
# define ROAD_H_

class		Road
{
private:
	std::string								_name;
	int										_priority;
	int										_maxSpeed;
	int										_numLanes;
	bool									_oneway;
	std::vector<std::pair<double, double> >	_points;
public:

	Road()
	{
		this->_points = std::vector<std::pair<double, double> >();
	}
	Road(const std::string& name, const int priority, const int maxSpeed, const int numLanes, const bool oneway)
	: _name(name), _priority(priority), _maxSpeed(maxSpeed), _numLanes(numLanes), _oneway(oneway)
	{
		this->_points = std::vector<std::pair<double, double> >();
	}

	void					AddPoints(const std::pair<double, double> &pair)
	{
		this->_points.push_back(pair);
	}

	void									setName(const std::string& name) { this->_name = name; };
	void									setPriority(const int priority) { this->_priority = priority; };
	void									setMaxSpeed(const int maxSpeed) { this->_maxSpeed = maxSpeed; };
	void									setNumLanes(const int numLanes) { this->_numLanes = numLanes; };
	void									setOneway(const bool oneway) { this->_oneway = oneway; };

	std::string								getName() const { return this->_name; };
	int										getPriority() const { return this->_priority; };
	int										getMaxSpeed() const { return this->_maxSpeed; };
	int										getNumLanes() const { return this->_numLanes; };
	bool									getOneway() const { return this->_oneway; };
	std::vector<std::pair<double, double> >	getPoints() const { return this->_points; };
};

#endif /* ROAD_H_ */
