#include "haversine.h"

int main(int argc, char* argv[]) {

  Haversine haversine(47.9840, 2.0300, 48.0080, 2.0080);

  double distance_miles = haversine.DistanceMiles();
  double distance_km = haversine.DistanceKm();

  // Output results
  std::cout << "Distance: " << std::endl;
  std::cout << "  " << std::setprecision(4) << distance_miles
  << " miles" << std::endl;
  std::cout << "  " << std::setprecision(4) << distance_km
  << " kilometers" << std::endl;
  return 0;
}
