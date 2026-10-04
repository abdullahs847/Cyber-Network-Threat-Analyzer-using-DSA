#ifndef NETWORK_GRAPH_H
#define NETWORK_GRAPH_H

#include <iostream>
#include <string>

using namespace std;

//STRUCTURES

struct Device;


struct Edge {
    Device* destination; 
    Edge* next;         
};


struct Device {
    string name;
    bool quarantined;
    int infectionCount;
    
    Edge* adjHead;  
    Device* next;    
    
    bool visited;    
};

//GRAPH CLASS 

class NetworkGraph {
private:
    Device* head; 
    const int MAX_DEVICES = 200;

public:
    NetworkGraph() : head(nullptr) {}

    Device* getHead() { return head; }

    Device* findDevice(string name) {
        Device* temp = head;
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void addDevice(string name) {
        if (findDevice(name)) {
            cout << "Device " << name << " already exists.\n";
            return;
        }
        
        Device* newDevice = new Device;
        newDevice->name = name;
        newDevice->quarantined = false;
        newDevice->infectionCount = 0;
        newDevice->adjHead = nullptr;
        newDevice->next = nullptr;
        newDevice->visited = false;

        if (!head) {
            head = newDevice;
        } else {
            Device* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newDevice;
        }
        cout << "Device " << name << " added successfully!\n";
    }

    void addConnection(string from, string to) {
        Device* source = findDevice(from);
        Device* dest = findDevice(to);

        if (!source || !dest) {
            cout << "One or both devices do not exist. Cannot connect.\n";
            return;
        }

        if (source == dest) { 
            cout << "Cannot connect a device to itself.\n";
            return;
        }

        Edge* current = source->adjHead; //for checking duplicate connection
        while (current != nullptr) {
            if (current->destination == dest) {
                cout << "Connection " << from << " -> " << to << " already exists. Skipped.\n";
                return; 
            }
            current = current->next;
        }

        Edge* newEdge = new Edge;
        newEdge->destination = dest;
        newEdge->next = nullptr;
        
        if (source->adjHead == nullptr) {
            source->adjHead = newEdge;
        } else {
            Edge* temp = source->adjHead;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newEdge;
        }
        cout << "Connection added: " << from << " -> " << to << endl;
    }

    void quarantineDevice(string name) {
        Device* target = findDevice(name);
        if (!target) {
            cout << "Device not found.\n";
            return;
        }

        target->quarantined = true;

        Edge* curr = target->adjHead;
        while (curr) {// outgoing
            Edge* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
        target->adjHead = nullptr;

        
        Device* temp = head;
        while (temp) { //incoming
            if (temp != target && temp->adjHead) {
                if (temp->adjHead->destination == target) {
                    Edge* toDelete = temp->adjHead;
                    temp->adjHead = temp->adjHead->next;
                    delete toDelete;
                } else {
                    Edge* e = temp->adjHead;
                    while (e->next) {
                        if (e->next->destination == target) {
                            Edge* toDelete = e->next;
                            e->next = e->next->next;
                            delete toDelete;
                            break; 
                        }
                        e = e->next;
                    }
                }
            }
            temp = temp->next;
        }
        cout << "Device " << name << " quarantined and disconnected.\n";
    }

    void showDevices() {
        if (!head) {
            cout << "No devices in the network.\n";
            return;
        }
        cout << "\nDevices in Network: ";
        Device* temp = head;
        while (temp) {
            cout << temp->name;
            if (temp->quarantined) cout << " [Q]";
            if (temp->infectionCount > 0) cout << " [INF]";
            if (temp->next) cout << " -> ";
            temp = temp->next;
        }
        cout << "\n";
    }

    void showConnections() {
        if (!head) {
            cout << "No devices.\n";
            return;
        }
        cout << "\nNetwork Connections (Adjacency List):\n";
        Device* temp = head;
        while (temp) {
            Edge* e = temp->adjHead;
            while (e) {
                cout << temp->name << " -> " << e->destination->name << endl;
                e = e->next;
            }
            temp = temp->next;
        }
    }
    
    void showQuarantined() {
        Device* temp = head;
        bool any = false;
        cout << "\nQuarantined Devices:\n";
        while (temp) {
            if (temp->quarantined) {
                cout << "- " << temp->name << "\n";
                any = true;
            }
            temp = temp->next;
        }
        if (!any) cout << "None\n";
    }

    bool hasDirectConnection(string a, string b) {
        Device* nodeA = findDevice(a);
        if(!nodeA) return false;
        
        Edge* e = nodeA->adjHead;
        while(e) {
            if(e->destination->name == b) return true;
            e = e->next;
        }
        return false;
    }

    void resetVisited() {
        Device* temp = head;
        while(temp) {
            temp->visited = false;
            temp = temp->next;
        }
    }

    int fillNameArray(string names[]) {
        int idx = 0;
        Device* temp = head;
        while (temp && idx < MAX_DEVICES) {
            names[idx++] = temp->name;
            temp = temp->next;
        }
        return idx;
    }
};

#endif