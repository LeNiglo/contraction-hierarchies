#include "haversine.h"

Haversine::Haversine(double lat_1, double lon_1, double lat_2, double lon_2)
{
	_lat_1 = lat_1 * (PI / 180);
	_lon_1 = lon_1 * (PI / 180);
	_lat_2 = lat_2 * (PI / 180);
	_lon_2 = lon_2 * (PI / 180);

	double delta_lat = _lat_1 - _lat_2;
	double delta_lon = _lon_1 - _lon_2;

	double sdlat = std::pow( std::sin(delta_lat / 2), 2);
	double sdlon = std::pow( std::sin(delta_lon / 2), 2);

	result = std::sqrt(sdlat + ( std::cos(lat_1) * std::cos(lat_2) * sdlon));
}

double Haversine::DistanceKm() {
	return 2 * radius_earth_km * std::asin(result);
}

double Haversine::DistanceMeters() {
	return 2 * radius_earth_meters * std::asin(result);
}

double Haversine::DistanceMiles() {
	return 2 * radius_earth_miles * std::asin(result);
}
