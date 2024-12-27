#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Coord {
public:
    double latitude; /* North-South coordinate: range (-90,90) */
    double longitude; /* East-West coordinate: range (-180,180) */
    char latitudeDir;
    char longitudeDir;

    /* y=Latitude, x=Longitude */
    Coord(const double y, const double x) {
        latitude = y;
        longitude = x;

        latitudeDir = (latitude >= 0) ? 'N' : 'S';
        longitudeDir = (longitude >= 0) ? 'E' : 'W';
    }

    Coord(const Coord &other) {
        latitude = other.latitude;
        longitude = other.longitude;

        latitudeDir = (latitude >= 0) ? 'N' : 'S';
        longitudeDir = (longitude >= 0) ? 'E' : 'W';
    }

    Coord() {
        latitude = 0;
        longitude = 0;

        latitudeDir = 'N';
        longitudeDir = 'E';
    }

    void printCoord() {
        printf("(%.5f°%c, %.5f°%c)", std::abs(latitude), latitudeDir, std::abs(longitude), longitudeDir);
    }
};

std::ostream& operator<<(std::ostream &stream, const Coord &coord) {
    return stream << '(' << std::abs(coord.latitude) << "°" << coord.latitudeDir << ", " << std::abs(coord.longitude) << "°" << coord.longitudeDir << ')';
}

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

    MapData(const std::string &fileName) {
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

    double get(const std::pair<int,int> &position) {
        return get(position.first, position.second);
    }

    double get(int x, int y) {
        return map[y][x];
    }

    Coord toCoordinate(const std::pair<int, int> &position) {
        return toCoordinate(position.first, position.second);
    }

    Coord toCoordinate(int x, int y) {
        return Coord(y_ur - cellSize * y, x_ll + cellSize * x);
    }

    std::pair<int, int> closestDataPoint(const Coord &coord) {
        return std::make_pair(
            std::round((coord.longitude - x_ll) / cellSize),
            std::round((y_ur - coord.latitude) / cellSize)
        );
    }
};
