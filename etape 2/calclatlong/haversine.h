#ifndef HARVERSINE_H_
# define HARVERSINE_H_

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>

class Haversine {
private:
  double _lat_1, _lon_1, _lat_2, _lon_2;
  double result;
  const double PI = 4.0 * atan(1.0);
  const double radius_earth_km = 6371.0088 ;

public:
  Haversine(double lat_1, double lon_1, double lat_2, double lon_2);
  // Returns the distancer in km.
  double		DistanceKm();
};

#endif /* HARVERSINE_H_ */
