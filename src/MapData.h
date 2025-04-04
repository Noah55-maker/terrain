#include <vector>
#include "Coord.h"

#pragma once

class MapData {
private:
    std::string unneeded;
public:
    int cols, rows;
    double x_ll, y_ll; /* lower left corner */
    double x_ur, y_ur; /* upper right corner */
    Coord llCoord, urCoord;
    double cellSize;
    int NO_DATA_VALUE;

    std::vector<std::vector<double> > map;

    bool isMissingData;

    /* 30 meters in 1 arc-second, 3600 arc-seconds in 1 degree */
    double data_separation;

    MapData(const std::string &fileName);

    double get(const std::pair<int,int> &position) {
        return get(position.first, position.second);
    }

    double get(int x, int y) {
        return map.at(y).at(x);
    }

    Coord toCoordinate(const std::pair<int, int> &position) {
        return toCoordinate(position.first, position.second);
    }

    Coord toCoordinate(int x, int y) {
        return Coord(y_ur - cellSize * y, x_ll + cellSize * x);
    }

    std::pair<int, int> closestDataPoint(const Coord &coord);
};
