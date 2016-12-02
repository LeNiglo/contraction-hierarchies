#ifndef PARSE_CSV_H_
#define PARSE_CSV_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "graph.h"

using namespace std;

double DistanceBetweenLatLngs(pair<double, double> ll1,
                              pair<double, double> ll2);

struct RoadData {
  Graph graph;
  vector<double> arc_durations;
  map<pair<double, double>, int> latlng_to_node;
};

RoadData ParseCsvFile(string filename);

#endif  // PARSE_CSV_H_
