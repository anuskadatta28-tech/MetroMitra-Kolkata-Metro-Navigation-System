#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
using namespace std;

// Structure to store station information
struct Station {
    string station_id;
    string station_name;
};

// Structure to store line information
struct Line {
    string line_id;
    string line_name;
    string line_color;
};

// Structure to store connection information
struct Connection {
    string from_station_id;
    string to_station_id;
    string line_id;
    double distance_km;
    int travel_time_min;
};

// Structure to store interchange information
struct Interchange {
    string station_id;
    string from_line_id;
    string to_line_id;
    int transfer_time_min;
};

// Structure to store graph edges
struct Edge {
    int destination;
    double distance_km;
    int travel_time_min;
};

// Structure to represent a station on a particular metro line
struct Node {
    string station_id;
    string line_id;
};

// Class to represent the metro graph
class Graph {
public:
    vector<Node> nodes;
    vector<vector<Edge>> adjList;

    // Constructor
    Graph(const vector<Node>& graphNodes) {
        nodes = graphNodes;
        adjList.resize(nodes.size());
    }

    // Function to add an edge between two nodes
    void addEdge(int source, int destination,
                double distance, int travelTime) {
        adjList[source].push_back({
            destination, distance, travelTime
        });
        adjList[destination].push_back({
            source, distance, travelTime
        });
    }

    // Function to find the index of a station-line node
    int getNodeIndex(string stationID, string lineID) {
        for (int i = 0; i < nodes.size(); i++) {

            if (nodes[i].station_id == stationID &&
                nodes[i].line_id == lineID) {

                return i;
            }
        }
        return -1;
    }

    // Function to add a directed interchange edge
    void addDirectedEdge(int source, int destination,
                        int transferTime) {

        adjList[source].push_back({
            destination, 0.0, transferTime
        });
    }

    // Dijkstra's Algorithm - Find fastest route
    vector<int> shortestPath(string sourceID, string destinationID) {
        int n = nodes.size();
        vector<int> dist(n, numeric_limits<int>::max());
        vector<int> parent(n, -1);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        // Start from all possible source line-nodes
        for (int i = 0; i < n; i++) {
            if (nodes[i].station_id == sourceID) {
                dist[i] = 0;
                pq.push({0, i});
            }
        }

        int destinationNode = -1;
        while (!pq.empty()) {
            int currentDistance = pq.top().first;
            int currentNode = pq.top().second;
            pq.pop();
            if (currentDistance > dist[currentNode]) {
                continue;
            }

            // Check whether destination is reached
            if (nodes[currentNode].station_id == destinationID) {
                destinationNode = currentNode;
                break;
            }

            for (const Edge& edge : adjList[currentNode]) {
                int newDistance = currentDistance + edge.travel_time_min;
                if (newDistance < dist[edge.destination]) {
                    dist[edge.destination] = newDistance;
                    parent[edge.destination] = currentNode;
                    pq.push({newDistance, edge.destination});
                }
            }
        }

        if (destinationNode == -1) {
            return {};
        }

        vector<int> path;
        for (int node = destinationNode; node != -1; node = parent[node]) {
            path.push_back(node);
        }

        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    vector<Station> stations;
    vector<Line> lines;
    vector<Connection> connections;
    vector<Interchange> interchanges;
    vector<Node> graphNodes;

    // Open the station dataset
    ifstream file("dataset/stations.csv");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open stations.csv" << endl;
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
    
    // Open line dataset
    ifstream lineFile("dataset/lines.csv");
    if (!lineFile.is_open()) {
        std::cout << "Error: Unable to open lines.csv" << endl;
        return 1;
    }
    string lineData;
    // Skip header
    getline(lineFile, lineData);
    while (getline(lineFile, lineData)) {
        stringstream ss(lineData);
        Line l;
        getline(ss, l.line_id, ',');
        getline(ss, l.line_name, ',');
        getline(ss, l.line_color, ',');
        lines.push_back(l);
    }
    lineFile.close();

    // Open connections dataset
    ifstream connectionFile("dataset/connections.csv");
    if (!connectionFile.is_open()) {
        std::cout << "Error: Unable to open connections.csv" << endl;
        return 1;
    }
    string connectionData;
    // Skip CSV header
    getline(connectionFile, connectionData);
    // Read connection records
    while (getline(connectionFile, connectionData)) {
        stringstream ss(connectionData);
        Connection c;
        getline(ss, c.from_station_id, ',');
        getline(ss, c.to_station_id, ',');
        getline(ss, c.line_id, ',');
        string distance, time;
        getline(ss, distance, ',');
        getline(ss, time, ',');
        c.distance_km = stod(distance);
        c.travel_time_min = stoi(time);
        connections.push_back(c);
    }
    connectionFile.close();

    // Open interchanges dataset
    ifstream interchangeFile("dataset/interchanges.csv");
    if (!interchangeFile.is_open()) {
        std::cout << "Error: Unable to open interchanges.csv" << endl;
        return 1;
    }
    string interchangeData;
    // Skip CSV header
    getline(interchangeFile, interchangeData);
    // Read interchange records
    while (getline(interchangeFile, interchangeData)) {
        stringstream ss(interchangeData);
        Interchange i;
        getline(ss, i.station_id, ',');
        getline(ss, i.from_line_id, ',');
        getline(ss, i.to_line_id, ',');

        string transferTime;
        getline(ss, transferTime, ',');
        i.transfer_time_min = stoi(transferTime);
        interchanges.push_back(i);
    }
    interchangeFile.close();
    
    // Create unique station-line nodes
    for (const Connection& c : connections) {
        bool fromFound = false;
        bool toFound = false;
        for (const Node& n : graphNodes) {
            if (n.station_id == c.from_station_id &&
                n.line_id == c.line_id) {
                fromFound = true;
            }
            if (n.station_id == c.to_station_id &&
                n.line_id == c.line_id) {
                toFound = true;
            }
        }
        if (!fromFound) {
            graphNodes.push_back({c.from_station_id, c.line_id});
        }
        if (!toFound) {
            graphNodes.push_back({c.to_station_id, c.line_id});
        }
    }

    // std::cout << "\n===== GRAPH INFORMATION =====" << endl;
    // std::cout << "Total Graph Nodes: " << graphNodes.size() << endl;
    // std::cout << "Graph Created Successfully!" << endl;

    // Create metro graph
    Graph metroGraph(graphNodes);
    // Add connections to the graph
    for (const Connection& c : connections) {
        int source = metroGraph.getNodeIndex(
            c.from_station_id, c.line_id
        );
        int destination = metroGraph.getNodeIndex(
            c.to_station_id, c.line_id
        );
        if (source != -1 && destination != -1) {
            metroGraph.addEdge(
                source,
                destination,
                c.distance_km,
                c.travel_time_min
            );
        }
    }

    // Add interchange edges to the graph
    for (const Interchange& i : interchanges) {
        int source = metroGraph.getNodeIndex(
            i.station_id, i.from_line_id
        );
        int destination = metroGraph.getNodeIndex(
            i.station_id, i.to_line_id
        );
        if (source != -1 && destination != -1) {
            metroGraph.addDirectedEdge(
                source,
                destination,
                i.transfer_time_min
            );
        }
    }
    
    // Take source and destination from user
    string sourceName, destinationName;
    std::cout << "\n===== PLAN YOUR JOURNEY =====" << endl;
    std::cout << "Enter Source Station: ";
    getline(cin, sourceName);
    std::cout << "Enter Destination Station: ";
    getline(cin, destinationName);

    // Convert Station names to IDs
    string sourceID = "";
    string destinationID = "";
    for (const Station& s : stations) {
        if (s.station_name == sourceName) {
            sourceID = s.station_id;
        }
        if (s.station_name == destinationName) {
            destinationID = s.station_id;
        }
    }

    if (sourceID == "" || destinationID == "") {
        cout << "\nInvalid station name!" << endl;
        cout << "Please enter valid station names." << endl;
    }
    else if (sourceID == destinationID) {
        cout << "\nYou are already at your destination!" << endl;
    }
    else {
        std::cout << "\nSource ID: " << sourceID << endl;
        std::cout << "Destination ID: " << destinationID << endl;

        // Find the fastest route
        vector<int> path = metroGraph.shortestPath(sourceID, destinationID);
        if (path.empty()) {
            std::cout << "\nNo route found between these stations!" << endl;
        }
        else {
            std::cout << "\n===== FASTEST ROUTE =====" << endl;

            for (int i = 0; i < path.size(); i++) {
                string stationID = metroGraph.nodes[path[i]].station_id;
                for (const Station& s : stations) {
                    if (s.station_id == stationID) {
                        std::cout << s.station_name;
                        if (i != path.size() - 1) {
                            std::cout << " -> ";
                        }
                        break;
                    }
                }
            }
            std::cout << endl;
            
            // Calculate total distance and travel time
            double totalDistance = 0.0;
            int totalTime = 0;

            // Check every pair of consecutive nodes in the path
            for (int i = 0; i < path.size() - 1; i++) {
                Node from = metroGraph.nodes[path[i]];
                Node to = metroGraph.nodes[path[i + 1]];

                // Normal metro connection
                if (from.line_id == to.line_id) {
                    for (const Connection& c : connections) {
                        if ((c.from_station_id == from.station_id &&
                            c.to_station_id == to.station_id) ||
                            (c.from_station_id == to.station_id &&
                            c.to_station_id == from.station_id)) {
                            totalDistance += c.distance_km;
                            totalTime += c.travel_time_min;
                            break;
                        }
                    }
                }

                // Interchange connection
                else {
                    for (const Interchange& t : interchanges) {
                        if (t.station_id == from.station_id &&
                            t.from_line_id == from.line_id &&
                            t.to_line_id == to.line_id) {
                            totalTime += t.transfer_time_min;
                            break;
                        }
                    }
                }
            }

            // Calculate estimated fare
            int fare = ceil(totalDistance / 2.0) * 5;

            // Display journey details
            cout << "\n===== JOURNEY DETAILS =====" << endl;
            cout << "Total Distance: " << totalDistance << " km" << endl;
            cout << "Total Travel Time: " << totalTime << " minutes" << endl;
            cout << "Estimated Ticket Fare: Rs. " << fare << endl;
            
            cout << "\n===== LINE & INTERCHANGE DETAILS =====" << endl;
            string currentLine = "";
            for (int i = 0; i < path.size(); i++) {
                Node currentNode = metroGraph.nodes[path[i]];
                if (currentNode.line_id != currentLine) {
                    // Find the line name
                    for (const Line& l : lines) {
                        if (l.line_id == currentNode.line_id) {
                            if (currentLine == "") {
                                cout << "Start Line: " << l.line_name << endl;
                            }
                            else {
                                cout << "Interchange at: ";
                                for (const Station& s : stations) {
                                    if (s.station_id == currentNode.station_id) {
                                        cout << s.station_name << endl;
                                        break;
                                    }
                                }
                                cout << "Continue on: "
                                    << l.line_name << endl;
                                cout << endl;
                            }
                            break;
                        }
                    }
                    currentLine = currentNode.line_id;
                }
            }
        }
    }

    // // Display all stations
    // std::cout << endl;
    // std::cout << "===== MetroMitra =====" << endl;
    // std::cout << "Available Metro Stations:" << endl << endl;
    // for (const Station& s : stations) {
    //     std::cout << s.station_id << " - "
    //          << s.station_name << endl;
    // }
    // std::cout << "\nTotal Stations: " << stations.size() << endl;
    
    // // Display all lines
    // std::cout << "\nAvailable Metro Lines:\n";
    // for (const Line& l : lines) {
    //     std::cout << l.line_id << " - "
    //      << l.line_name << " - "
    //      << l.line_color << endl;
    // }
    // std::cout << "\nTotal Lines: " << lines.size() << endl;
    
    // // Display all connections
    // std::cout << "\nAvailable Metro Connections:\n";
    // for (const Connection& c : connections) {
    //     std::cout << c.from_station_id << " -> "
    //         << c.to_station_id << " | "
    //         << c.line_id << " | "
    //         << c.distance_km << " km | "
    //         << c.travel_time_min << " min"
    //         << endl;
    // }
    // std::cout << "\nTotal Connections: " << connections.size() << endl;
    
    // // Display all interchanges
    // std::cout << "\nAvailable Interchanges:\n";
    // for (const Interchange& i : interchanges) {
    //     std::cout << i.station_id << " | "
    //         << i.from_line_id << " -> "
    //         << i.to_line_id << " | "
    //         << i.transfer_time_min << " min"
    //         << endl;
    // }
    // std::cout << "\nTotal Interchanges: "
    //     << interchanges.size() << endl;

    return 0;
}

