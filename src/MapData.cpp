#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "MapData.h"

MapData::MapData(const std::string &fileName) {
    std::ifstream fin(fileName);

    fin >> unneeded >> cols >> unneeded >> rows;
    fin >> unneeded >> x_ll >> unneeded >> y_ll;
    fin >> unneeded >> cellSize;
    fin >> unneeded >> NO_DATA_VALUE; // "no data" value

    x_ur = x_ll + cellSize * (cols - 1);
    y_ur = y_ll + cellSize * (rows - 1);
    llCoord = Coord(y_ll, x_ll);
    urCoord = Coord(y_ur, x_ur);

    /* 30 meters in 1 arc-second, 3600 arc-seconds in 1 degree */
    data_separation = cellSize * 3600 * 30;

    map = std::vector<std::vector<double> >(rows);

    isMissingData = false;
    for (int i = 0; i < rows; i++) {
        map[i] = std::vector<double>(cols);

        for (int j = 0; j < cols; j++) {
            fin >> map[i][j];
            if (map[i][j] == NO_DATA_VALUE) {
                isMissingData = true;
            }
        }
    }
    if (isMissingData) {
        std::wcout << "Warning: missing data\n";
        std::wcout << "Proceed with caution\n";
    }

    fin.close();
}

std::pair<int, int> MapData::closestDataPoint(const Coord &coord) {
    return std::make_pair(
        std::round((coord.longitude - x_ll) / cellSize),
        std::round((y_ur - coord.latitude) / cellSize)
    );
}
