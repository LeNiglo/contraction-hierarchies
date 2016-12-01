#include "haversine.h"

int main(int argc, char* argv[]) {

  Haversine haversine(33, 1,-45,-9);

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
