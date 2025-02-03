#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>


// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya",
    "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string &distance_data, const std::string &priority_data,
                           const std::string &restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data);
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string &filename) {
    std::ifstream file(filename);

    std::string line;
    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        if (line.empty()) continue;

        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen == std::string::npos || closeParen == std::string::npos) {
            continue;
        }

        int provinceIndex = std::stoi(line.substr(openParen + 1, closeParen - openParen - 1));

        priorityProvinces[numPriorityProvinces++] = provinceIndex;
    }

    file.close();
}


// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string &filename) {
    std::ifstream file(filename);

    std::string line;
    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        if (line.empty()) continue;

        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen == std::string::npos || closeParen == std::string::npos) {
            continue;
        }

        int provinceIndex = std::stoi(line.substr(openParen + 1, closeParen - openParen - 1)); // İndeks

        weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = provinceIndex;
    }

    file.close();
}


// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numPriorityProvinces; i++) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    stack.push(startingCity);
    map.markAsVisited(startingCity);


    while (!isExplorationComplete()) {
        int currentCity = stack.peek();

        // Find neighbors of current city
        enqueueNeighbors(currentCity);

        // If there is no neighbor call backtrack function
        if(queue.isEmpty()) {
            backtrack();
        }

        while (!queue.isEmpty()) {
            int nextCity = queue.dequeue();

            if (!map.isVisited(nextCity)) {
                stack.push(nextCity);
                map.markAsVisited(nextCity);
                break;
            }
        }
    }

    for (int i = 0; i < 81; i++) {
        if (map.isVisited(i)) {
            route.push_back(i);
        }
    }

    displayResults();
}


// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    while (!queue.isEmpty()) {
        queue.dequeue();
    }

    map.markAsVisited(province);

    for (int i = 0; i < 81; i++) {
        if (i != province && !map.isVisited(i) && map.isWithinRange(province, i, maxDistance)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else if (!isWeatherRestricted(i)) {
                queue.enqueue(i);
            } else {
                std::cout << "Province " << cities[i] << " is weather-restricted. Skipping.\n";
            }
        }
    }
}

void RoutePlanner::backtrack() {
    if (!stack.isEmpty()) {
        stack.pop();
        if (!stack.isEmpty()) {
            int previousProvince = stack.peek();
            enqueueNeighbors(previousProvince);
        }
    }
}

bool RoutePlanner::isExplorationComplete() const {
    if (stack.isEmpty() && queue.isEmpty()) {
        return true;
    }
    return false;
}

void RoutePlanner::displayResults() const {
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken: ";

    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i != route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    std::cout << "\nPriority Provinces Status:\n";
    int visitedPriorityCount = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        std::cout << "- " << cities[province] << " ("
                  << (map.isVisited(province) ? "Visited" : "Not Visited") << ")\n";
        if (map.isVisited(province)) {
            visitedPriorityCount++;
        }
    }

    std::cout << "\nTotal Priority Provinces Visited: " << visitedPriorityCount
              << " out of " << numPriorityProvinces << std::endl;

    if (visitedPriorityCount < numPriorityProvinces) {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}



