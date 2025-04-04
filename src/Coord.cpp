#include "Coord.h"

Coord::Coord()  {
    latitude = 0;
    longitude = 0;

    latitudeDir = 'N';
    longitudeDir = 'E';
}

Coord::Coord(const Coord &other) {
    latitude = other.latitude;
    longitude = other.longitude;

    latitudeDir = (latitude >= 0) ? 'N' : 'S';
    longitudeDir = (longitude >= 0) ? 'E' : 'W';
}

Coord::Coord(const double y, const double x) {
    latitude = y;
    longitude = x;

    latitudeDir = (latitude >= 0) ? 'N' : 'S';
    longitudeDir = (longitude >= 0) ? 'E' : 'W';
}
