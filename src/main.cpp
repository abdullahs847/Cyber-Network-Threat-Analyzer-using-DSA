#include <iostream>
#include <limits>
#include <string>
#include "NetworkGraph.h"
#include "Simulator.h"
#include "NetworkAnalyzer.h" 

using namespace std;


int main() {
    NetworkGraph network;       // Manages nodes (devices) and edges (connections)
    Simulator sim;              // Manages BFS infection simulation
    NetworkAnalyzer analyzer;   // Manages specific analysis (DFS patient zero, neighbor check)

    int choice;
    string a, b;

    do {
        system("cls"); 
        cout << "\n---------- Cyber Network Threat Analyzer (Modular) ----------\n";
        cout << "1.  Add Device (Node)\n";
        cout << "2.  Show Devices\n";
        cout << "3.  Add Connection (Edge)\n";
        cout << "4.  Show Connections (Adjacency List)\n";
        cout << "5.  Simulate Infection Spread (BFS)\n";
        cout << "6.  Quarantine Device\n";
        cout << "7.  Show Quarantined Devices\n";
        cout << "8.  Show Behavior Scores\n";
        cout << "9.  Attack Prediction Engine\n";
        cout << "10. Check Neighbor Infection Status\n";    
        cout << "11. Locate Patient Zero (DFS Analysis)\n"; 
        cout << "12. Exit\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter device name: ";
            cin >> a;
            network.addDevice(a);
            break;

        case 2:
            network.showDevices();
            break;

        case 3:
            cout << "Enter connection (From To): ";
            cin >> a >> b;
            network.addConnection(a, b);
            break;

        case 4:
            network.showConnections();
            break;

        case 5:
            cout << "Enter starting infected device: ";
            cin >> a;
            sim.simulateInfection(network, a);
            break;

        case 6:
            cout << "Enter device to quarantine: ";
            cin >> a;
            network.quarantineDevice(a);
            break;

        case 7:
            network.showQuarantined();
            break;

        case 8:
            sim.showBehaviorScores(network);
            break;

        case 9:
            cout << "Enter currently infected device: ";
            cin >> a;
            if (!network.findDevice(a)) {
                cout << "Device not found.\n";
            } else {
                sim.attackPrediction(network, a);
            }
            break;

        case 10: 
            cout << "Enter device to check neighbors for: ";
            cin >> a;
            analyzer.checkConnectedInfection(network, a);
            break;

        case 11: 
            analyzer.locateInfectionSourceDFS(network);
            break;

        case 12:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

        if (choice != 12) {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (choice != 12);

    return 0;
}