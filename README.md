# MetroMitra – Kolkata Metro Navigation System

## 1. About the Project
MetroMitra is a C++-based metro navigation system designed for the Kolkata Metro network. It helps users find the fastest route between two metro stations by considering travel time.

The system provides the station sequence, metro lines, interchange stations, estimated journey time, total distance, and estimated ticket fare.

## 2. Objectives
- To find the fastest route between two metro stations.
- To display the complete station-by-station route.
- To identify metro lines and interchange stations.
- To calculate estimated journey time and distance.
- To calculate an estimated ticket fare.
- To store metro data separately using CSV files.

## 3. Key Features
- Source and destination station selection.
- Fastest route calculation.
- Complete station sequence.
- Metro line information.
- Interchange station identification.
- Estimated travel time and distance.
- Estimated ticket fare.

## 4. Technologies Used
- Programming Language: C++
- Concepts: Object-Oriented Programming (OOP)
- Data Structures: Graph, Adjacency List, Vector
- Algorithm: Dijkstra's Shortest Path Algorithm
- STL: Priority Queue, Vector, Unordered Map
- File Handling: CSV File Reading

## 5. Data Structures and Algorithms

| Concept | Purpose |
|---|---|
| Graph | Represents metro stations and connections |
| Adjacency List | Stores connected stations |
| Dijkstra's Algorithm | Finds the route with minimum travel time |
| Priority Queue | Efficiently selects the station with minimum travel time |
| Vector | Stores station and route information |
| Unordered Map | Maps station IDs to station details |
| OOP | Organizes the project into classes and objects |

## 6. Dataset Structure

The project uses separate CSV files to store metro network information.

**Dataset files:**

- `stations.csv` – Stores station IDs and station names.
- `lines.csv` – Stores metro line IDs, names, and colours.
- `connections.csv` – Stores connected stations, line IDs, distances, and travel times.
- `interchanges.csv` – Stores interchange stations, connecting lines, and transfer times.

## 7. Routing Algorithm

MetroMitra uses Dijkstra's Shortest Path Algorithm to find the fastest route.

- Each metro station and connection is represented using a weighted graph.
- Travel time is used as the edge weight.
- A priority queue helps select the station with the minimum accumulated travel time.
- Interchange time is also considered while calculating the route.

The route with the minimum total travel time is selected.

## 8. Fare Calculation

The project uses a simulated fare calculation model:

**Fare = ceil(Total Distance / 2) × ₹5**

The fare is calculated based on the total distance travelled, rounded up to the next 2 km interval.

*Note: This is an academic fare model and does not represent the official Kolkata Metro fare structure.*

## 9. Dataset Disclaimer

The project uses a structured dataset of Kolkata Metro stations, lines, connections, and interchanges.

Some distance, travel-time, and transfer-time values are approximate or simulated for academic implementation. They should not be treated as official real-time metro data.

## 10. Team Members

| Name | UID |
|---|---|
| Anuska Datta | 25BCS10364 |
| Bhavya Sharma | 25BCS12027 |
| Kanika Saini | 25BCS12309 |

## 11. Project Status

- [x] GitHub repository created
- [x] Dataset CSV files prepared
- [ ] CSV file handling in C++
- [ ] Graph construction
- [ ] Dijkstra's algorithm implementation
- [ ] Route and fare calculation
- [ ] User interface

## 12. Future Scope
- Interactive graphical user interface.
- Real-time metro updates.
- Live train arrival information.
- More accurate official fare integration.
- Route suggestions based on different user preferences.
