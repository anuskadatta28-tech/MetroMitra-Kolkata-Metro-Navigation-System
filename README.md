# MetroMitra – Kolkata Metro Navigation System

**MetroMitra** is a C++-based metro navigation system designed to help users find the fastest route between two metro stations in the Kolkata Metro network.
The project uses graph data structures and Dijkstra's Shortest Path Algorithm to calculate the minimum-time route. It also provides the station sequence, metro lines, interchange information, estimated journey time, distance and ticket fare.

## Project Objectives
- Find the fastest route between two metro stations.
- Display the complete sequence of stations.
- Identify metro lines and interchange stations.
- Calculate estimated journey time and distance.
- Calculate an estimated ticket fare.
- Store metro network data separately using CSV files.

## Key Features
- **Fastest Route:** Finds the route with minimum travel time.
- **Station Search:** Accepts source and destination station names.
- **Route Display:** Shows the sequence of stations from source to destination.
- **Interchange Detection:** Identifies when passengers need to change metro lines.
- **Journey Details:** Displays total distance and estimated travel time.
- **Fare Estimation:** Calculates an estimated ticket fare.
- **Input Validation:** Handles invalid station names and identical source and destination.
- **CSV Data Management:** Reads station and metro network information from external files.

## Data Structures and Algorithms Used

| Concept | Application |
|---|---|
| Graph | Represents the metro network |
| Adjacency List | Stores connections between stations |
| Weighted Edges | Stores distance and travel time |
| Dijkstra's Algorithm | Finds the fastest route |
| Priority Queue | Selects the node with minimum travel time |
| Vector | Stores stations, graph nodes and paths |
| Structures | Represents stations, lines, connections and edges |
| OOP | Implements the Graph class |
| File Handling | Reads CSV datasets |

## Technology Stack
- **Programming Language:** C++
- **Compiler:** GCC (g++)
- **IDE:** Visual Studio Code
- **Data Storage:** CSV files
- **Version Control:** GitHub

## Project Structure

```text
MetroMitra-Kolkata-Metro-Navigation-System/
│
├── dataset/
│   ├── stations.csv
│   ├── lines.csv
│   ├── connections.csv
│   └── interchanges.csv
│
├── src/
│   └── main.cpp
│
├── .gitignore
└── README.md
```

## Dataset Description
The project uses separate CSV files to store metro network information.

| File | Description |
|---|---|
| stations.csv | Station IDs and station names |
| lines.csv | Metro line IDs, names and colours |
| connections.csv | Connected stations, line IDs, distances and travel times |
| interchanges.csv | Interchange stations and transfer times |

The current dataset contains:
- 55 station records
- 5 metro lines
- 52 connection records
- 4 directional interchange records
- 57 station-line graph nodes

## Routing Methodology
MetroMitra uses a **weighted graph** to represent the metro network.
Each graph node represents a station-line combination. This allows the system to distinguish between different metro lines at an interchange station.

The routing process follows these steps:
1. Read metro information from CSV files.
2. Create graph nodes for station-line combinations.
3. Build the adjacency list using metro connections.
4. Add directed edges for interchange transitions.
5. Apply Dijkstra's algorithm using travel time as the edge weight.
6. Reconstruct and display the fastest route.
7. Calculate the total distance, journey time and estimated fare.

## Fare Calculation
The project uses a simulated fare calculation rule:
      **Fare = ceil(Total Distance / 2) × ₹5**
      
The fare is rounded up to the next 2 km interval.
*Note: This is an academic simulation and does not represent the official Kolkata Metro fare structure.*

## How to Run the Project
### Prerequisites
- GCC C++ compiler installed.
- Visual Studio Code or any C++ IDE.

### Compilation
Open the terminal in the project root directory and execute:
```bash
g++ src/main.cpp -o MetroMitra
```
### Execution
On Windows PowerShell:
```powershell
.\MetroMitra.exe
```
Enter the source and destination station names when prompted.

## Dataset Limitations
- The current dataset contains estimated travel times, distances and transfer times. 
- Some metro sections are not yet connected to the main network in the dataset.
- Therefore, route availability and calculated values are dependent on the available CSV records.
- The project is an academic navigation simulation, not an official real-time metro service.

## Future Scope
- Develop a graphical user interface using HTML, CSS and JavaScript.
- Add an interactive metro network map.
- Expand and verify the metro dataset.
- Add alternative route suggestions.
- Include real-time metro information if a reliable data source becomes available.

## Team Members

| Name | UID |
|---|---|
| Anuska Datta | 25BCS10364 |
| Bhavya Sharma | 25BCS12027 |
| Kanika Saini | 25BCS12309 |

## Project Information
- Project Name: MetroMitra – Kolkata Metro Navigation System
- Domain: Data Structures and Algorithms (ADSA)
- Language: C++
- Repository: https://github.com/anuskadatta28-tech/MetroMitra-Kolkata-Metro-Navigation-System

Developed as an academic project to apply graph algorithms, data structures, OOP concepts and file handling in a practical navigation system.
