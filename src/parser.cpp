// Data from https://portal.opentopography.org/apidocs/#/Public/getUsgsDem
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

const std::string fileName = "larger-data.asc";
const int data_separation = 30; // 30 meters = 1 arc-second
const int NO_DATA_VALUE = -999999;

std::string unneeded;

class MapData {
public:
    int cols, rows;
    double x_ll, y_ll; // lower left corner
    double x_ur, y_ur;
    double cellSize;
    std::vector<std::vector<double> > map;
    std::vector<std::vector<int> > mapMeters;

    bool isMissingData;

    // double data_separation = 1 / cellSize;

    MapData(std::string fileName) {
        std::ifstream fin(fileName);

        fin >> unneeded >> cols >> unneeded >> rows;
        fin >> unneeded >> x_ll >> unneeded >> y_ll;
        fin >> unneeded >> cellSize;
        fin >> unneeded >> unneeded; // "no data" value

        x_ur = x_ll + cellSize * cols;
        y_ur = y_ll + cellSize * rows;

        map = std::vector<std::vector<double> >(rows);
        mapMeters = std::vector<std::vector<int> >(rows);

        isMissingData = false;
        for (int i = 0; i < rows; i++) {
            map[i] = std::vector<double>(cols);
            mapMeters[i] = std::vector<int>(cols);

            for (int j = 0; j < cols; j++) {
                fin >> map[i][j];
                mapMeters[i][j] = (int) (map[i][j] * data_separation);
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

    std::pair<double, double> toCoordinate(std::pair<int, int> position) {
        return toCoordinate(position.first, position.second);
    }

    std::pair<double, double> toCoordinate(int x, int y) {
        return std::make_pair(x_ll + cellSize * x, y_ur - cellSize * y);
    }
};

void printCoord(std::pair<double, double> coord) {
    char longitudeDir = (coord.first >= 0) ? 'E' : 'W';
    char lattitudeDir = (coord.second >= 0) ? 'N' : 'S';

    printf("(%.5f %c, %.5f %c)", std::abs(coord.second), lattitudeDir, std::abs(coord.first), longitudeDir);
}

void printPosition(MapData map, int i, int j) {
    printCoord(map.toCoordinate(j, i));
}

int main(int argc, char **argv) {
    MapData map(fileName);

    std::cout << "Lower Left: ";
    printCoord(std::make_pair(map.x_ll, map.y_ll));
    std::cout << '\n' << "Upper Right: ";
    printCoord(std::make_pair(map.x_ur, map.y_ur));
    std::cout << "\n\n";


    double minVal = -NO_DATA_VALUE, maxVal = NO_DATA_VALUE;
    std::pair<int, int> minPos, maxPos;
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            if (map.map[i][j] < minVal) {
                minVal = map.map[i][j];
                minPos = std::make_pair(j, i);
            }
            if (map.map[i][j] > maxVal) {
                maxVal = map.map[i][j];
                maxPos = std::make_pair(j, i);
            }
        }
    }

    std::cout << map.map[0][0] << ' ' << map.map[0][map.cols-1] << '\n';
    std::cout << map.map[map.rows-1][0] << ' ' << map.map[map.rows-1][map.cols-1] << '\n';


    std::cout << '\n';
    std::cout << "min: " << minVal << ", max: " << maxVal << '\n';

    std::pair<double,double> minCoord = map.toCoordinate(minPos);
    std::pair<double,double> maxCoord = map.toCoordinate(maxPos);

    printf("minPos: x=%d, y=%d\th=%f\n", minPos.first, minPos.second, minVal);
    printf("minCoord: "); printCoord(minCoord); printf("\n");
    printf("maxPos: x=%d, y=%d\th=%f\n", maxPos.first, maxPos.second, maxVal);
    printf("maxCoord: "); printCoord(maxCoord); printf("\n");

    return 0;
}
