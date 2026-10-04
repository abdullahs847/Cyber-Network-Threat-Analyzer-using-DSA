#ifndef NETWORK_ANALYZER_H
#define NETWORK_ANALYZER_H

#include <iostream>
#include <string>
#include "NetworkGraph.h"

using namespace std;

class NetworkAnalyzer {
private:
    void dfsReachability(Device* current, int& count) {
        current->visited = true;
        count++;
        Edge* e = current->adjHead;
        while (e) {
            
            if (!e->destination->visited && e->destination->infectionCount > 0) {
                dfsReachability(e->destination, count);
            }
            e = e->next;
        }
    }

public:
    
    void checkConnectedInfection(NetworkGraph& graph, string name) {
        Device* d = graph.findDevice(name);
        if (!d) {
            cout << "Device not found.\n";
            return;
        }
        cout << "\n--- Neighbor Infection Check for " << name << " ---\n";
        Edge* e = d->adjHead;
        bool hasNeighbors = false;
        while (e) {
            hasNeighbors = true;
            cout << "Neighbor: " << e->destination->name;
            
            if (e->destination->infectionCount > 0) 
                cout << " [STATUS: INFECTED]";
            else 
                cout << " [STATUS: CLEAN]";
            
            if (e->destination->quarantined)
                cout << " (Quarantined)";
            
            cout << endl;
            e = e->next;
        }
        if (!hasNeighbors) cout << "No connected devices found.\n";
        cout << "------------------------------------------\n";
    }

    
    void locateInfectionSourceDFS(NetworkGraph& graph) {
        Device* potentialZero = nullptr;
        int maxReachable = -1;

        Device* temp = graph.getHead();
        bool anyInfected = false;

    
        while (temp) {
            if (temp->infectionCount > 0) {
                anyInfected = true;
                
                
                graph.resetVisited();
                
                int count = 0;
                dfsReachability(temp, count);
              
                if (count > maxReachable) {
                    maxReachable = count;
                    potentialZero = temp;
                }
            }
            temp = temp->next;
        }

        if (anyInfected && potentialZero) {
            cout << "\n--- Patient Zero Analysis (DFS) ---\n";
            cout << "Potential Infection Source: " << potentialZero->name << endl;
            cout << "Impact Scope: Can reach " << maxReachable << " other infected device(s).\n";
        } else {
            cout << "No active infections found in the network to analyze.\n";
        }
    }
};

#endif