#pragma once

class Coord {
public:
    double latitude; /* North-South coordinate: range (-90,90) */
    double longitude; /* East-West coordinate: range (-180,180) */
    char latitudeDir;
    char longitudeDir;

    Coord();

    Coord(const Coord &other);

    /* y=Latitude, x=Longitude */
    Coord(const double y, const double x);
};
