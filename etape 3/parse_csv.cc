#include <cstdio>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <set>
using namespace std;

#include "base.h"
#include "parse_csv.h"

double DistanceBetweenLatLngs(pair<double, double> ll1, pair<double, double> ll2) {
	double lat1 = ll1.first;
	double lng1 = ll1.second;
	double lat2 = ll2.first;
	double lng2 = ll2.second;
	// Convert to radians...
	const double kDegToRad = 3.1415926535897932384626433 / 180;
	lat1 *= kDegToRad;
	lng1 *= kDegToRad;
	lat2 *= kDegToRad;
	lng2 *= kDegToRad;
	// https://en.wikipedia.org/wiki/Haversine_formula
	const double s1 = sin(0.5 * (lat2 - lat1));
	const double s2 = sin(0.5 * (lng2 - lng1));
	const double d = 2 * asin(sqrt(s1 * s1 + cos(lat1) * cos(lat2) * s2 * s2));
	// https://en.wikipedia.org/wiki/Earth_radius#Mean_radius
	const double kMeanEarthRadiusMeters = 6371008.8;
	return kMeanEarthRadiusMeters * d;
}

RoadData ParseCsvFile(string filename) {
	ifstream is(filename);
	string line;
	struct Road {
		bool oneway;
		int speed_kmh;
		vector<pair<double, double>> latlngs;
	};

	vector<Road> roads;
	while (getline(is, line))
	{
		vector<string> fields;
		string field;
		istringstream is(line);
		while (getline(is, field, ',')) fields.push_back(field);
		int numpoints = stoi(fields[5]);
		CHECK_EQ(2 * numpoints + 6, fields.size()) << line;
		Road road;
		for (int i = 0; i < numpoints; ++i)
		{
			double lat, lng;
			CHECK_EQ(sscanf(fields[2 * i + 6].c_str(), "%lf", &lat), 1);
			CHECK_EQ(sscanf(fields[2 * i + 7].c_str(), "%lf", &lng), 1);
			road.latlngs.push_back(make_pair(lat, lng));
		}
		int oneway_int;
		sscanf(fields[4].c_str(), "%d", &oneway_int);
		road.oneway = oneway_int == 1;
		sscanf(fields[2].c_str(), "%d", &road.speed_kmh);
		roads.push_back(road);
	}

	// Determine which lat/lng are in fact nodes.
	map<pair<double, double>, int> latlng_count;
	set<pair<double, double>> latlng_nodes;
	for (int i = 0; i < roads.size(); ++i)
	{
		const Road& road = roads[i];
		for (int j = 0; j < road.latlngs.size(); ++j)
		{
			pair<double, double> ll = road.latlngs[j];
			if (++latlng_count[ll] > 1) latlng_nodes.insert(ll);
		}
		latlng_nodes.insert(road.latlngs[0]);
		latlng_nodes.insert(road.latlngs.back());
	}

	// Map between lat/lng and node index.
	map<pair<double, double>, int> latlng_to_node;
	int num_nodes = 0;
	for (set<pair<double, double>>::const_iterator it = latlng_nodes.begin();
	it != latlng_nodes.end(); ++it)
	{
		latlng_to_node[*it] = num_nodes++;
	}
	cout << num_nodes << endl;  // Print num_nodes

	Graph graph;
	vector<double> arc_durations;
	for (int r = 0; r < roads.size(); ++r)
	{
		const Road& road = roads[r];
		int i = 0;
		while (i < road.latlngs.size() - 1)
		{
			int j = i + 1;
			while (latlng_nodes.count(road.latlngs[j]) == 0) j++;
			double length = 0;
			for (int k = i; k < j; ++k)
			{
				length += DistanceBetweenLatLngs(road.latlngs[k], road.latlngs[k + 1]);
			}
			const double duration = length * 3.6 / road.speed_kmh;
			graph.AddArc(latlng_to_node[road.latlngs[i]], latlng_to_node[road.latlngs[j]]);
			arc_durations.push_back(duration);
			if (!road.oneway)
			{
				graph.AddArc(latlng_to_node[road.latlngs[j]], latlng_to_node[road.latlngs[i]]);
				arc_durations.push_back(duration);
			}
			i = j;
		}
	}
	cout << arc_durations.size() << endl;  // Print num_arcs
	double duration_sum = 0;
	for (int i = 0; i < arc_durations.size(); ++i)
	{
		duration_sum += arc_durations[i];
	}
	printf("%lf\n", duration_sum);

	RoadData data;
	data.graph = graph;
	data.latlng_to_node = latlng_to_node;
	data.arc_durations = arc_durations;
	return data;
}
