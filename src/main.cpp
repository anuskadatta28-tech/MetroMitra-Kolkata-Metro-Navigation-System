#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Structure to store station information
struct Station {
    string station_id;
    string station_name;
};

int main() {
    vector<Station> stations;

    // Open the station dataset
    ifstream file("dataset/stations.csv");

    if (!file.is_open()) {
        cout << "Error: Unable to open stations.csv" << endl;
        return 1;
    }

    string line;
    // Skip the CSV header
    getline(file, line);

    // Read station records
    while (getline(file, line)) {
        stringstream ss(line);
        Station s;
        getline(ss, s.station_id, ',');
        getline(ss, s.station_name, ',');
        stations.push_back(s);
    }

    file.close();

    // Display all stations
    cout << endl;
    cout << "===== MetroMitra =====" << endl;
    cout << "Available Metro Stations:" << endl << endl;

    for (const Station& s : stations) {
        cout << s.station_id << " - "
             << s.station_name << endl;
    }

    cout << "\nTotal Stations: " << stations.size() << endl;

    return 0;
}

