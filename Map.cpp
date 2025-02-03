#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits>
#include <map>

Map::Map() {
    // TODO: Your code here

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            if (i == j) {
                distanceMatrix[i][j] = 0;
            } else {
                distanceMatrix[i][j] = std::numeric_limits<int>::max();
            }
        }
        visited[i] = false;

    }

}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here

    // Read each line in the CSV file
    std::ifstream file(filename);

    std::string line;
    int row = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (std::getline(ss, cell, ',')) {
            distanceMatrix[row][col] = std::stoi(cell);
            col++;
        }
        row++;
    }
    file.close();


}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    if (provinceA < 0 || provinceA >= MAX_SIZE || provinceB < 0 || provinceB >= MAX_SIZE) {
        return false;
    }
    return distanceMatrix[provinceA][provinceB] <= maxDistance;

}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if (province >= 0 && province < MAX_SIZE) {
        return visited[province];
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i]) {
            count++;
        }
    }
    if (count != 0) {
        return count;
    }

    return 0;


}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    if(distanceMatrix[provinceA][provinceB] != 0) {
        return distanceMatrix[provinceA][provinceB];
    }
    return 0;
}