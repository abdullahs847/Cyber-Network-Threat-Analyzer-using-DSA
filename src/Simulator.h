#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <string>
#include "NetworkGraph.h"
#include "CustomQueue.h"

using namespace std;

class Simulator {
private:
    const int MAX_DEVICES = 200;
public:
    void merge(int indices[], float arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = indices[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = indices[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (arr[L[i]] >= arr[R[j]]) {  // Descending order
            indices[k] = L[i];
            i++;
        } 
        else {
            indices[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        indices[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        indices[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSortIndicesDescending(float arr[], int indices[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortIndicesDescending(arr, indices, left, mid);
        mergeSortIndicesDescending(arr, indices, mid + 1, right);
        merge(indices, arr, left, mid, right);
    }
}


    void simulateInfection(NetworkGraph& graph, string startDevice) {
        Device* start = graph.findDevice(startDevice);
        if (!start) {
            cout << "Device not found.\n";
            return;
        }
        if (start->quarantined) {
            cout << "Cannot start infection from a quarantined device!\n";
            return;
        }

        cout << "\n--- Infection Spread Simulation (Graph BFS) ---\n";
        
        CustomQueue q;
        graph.resetVisited(); 
        
        q.enqueue(start);
        start->visited = true;

        while (!q.isEmpty()) {
            Device* current = q.dequeue();
            cout << current->name << " ";
            current->infectionCount++;

            // Traverse neighbors(Graph edges)
            Edge* neighbor = current->adjHead;
            while (neighbor) {
                Device* nextDev = neighbor->destination;
                
                if (!nextDev->visited && !nextDev->quarantined) {
                    nextDev->visited = true;
                    q.enqueue(nextDev);
                }
                
                neighbor = neighbor->next;
            }
        }
        cout << endl;
    }

    int buildDeviceStats(NetworkGraph& graph, string names[], int outgoing[], int incoming[], int wasInfected[]) {
        int n = graph.fillNameArray(names);
        
        for (int i = 0; i < n; ++i) {
            outgoing[i] = 0;
            incoming[i] = 0;
            wasInfected[i] = 0;
        }

        Device* d = graph.getHead();
        while(d) {
            int Idx = -1;
            for(int i=0; i<n; ++i) if(names[i] == d->name) Idx = i;

            Edge* e = d->adjHead;
            while(e) {
                if(Idx != -1) outgoing[Idx]++;

                string neighborName = e->destination->name;
                for(int i=0; i<n; ++i) {
                    if(names[i] == neighborName) {
                        incoming[i]++;
                        break;
                    }
                }
                e = e->next;
            }
            d = d->next;
        }

        for (int i = 0; i < n; ++i) {
            Device* dev = graph.findDevice(names[i]);
            if (dev) wasInfected[i] = dev->infectionCount;
        }

        return n;
    }

    void computeBehaviorPercentages(int n, int outgoing[], int incoming[], int wasInfected[], float behaviorPercent[]) {
        float scores[MAX_DEVICES];
        float maxScore = 0.0f;

        for (int i = 0; i < n; ++i) {
            float score = outgoing[i] * 5.0f + incoming[i] * 3.0f + wasInfected[i] * 8.0f;
            scores[i] = score;
            if (score > maxScore) maxScore = score;
        }

        if (maxScore <= 0.0f) {
            for (int i = 0; i < n; ++i) behaviorPercent[i] = 0.0f;
            return;
        }

        for (int i = 0; i < n; ++i) {
            behaviorPercent[i] = (scores[i] / maxScore) * 100.0f;
        }
    }

    void showBehaviorScores(NetworkGraph& graph) {
        string names[MAX_DEVICES];
        int outgoing[MAX_DEVICES], incoming[MAX_DEVICES], wasInfected[MAX_DEVICES];
        float behaviorPercent[MAX_DEVICES];
        int indices[MAX_DEVICES];

        int n = buildDeviceStats(graph, names, outgoing, incoming, wasInfected);
        if (n == 0) {
            cout << "No devices to analyze.\n";
            return;
        }

        computeBehaviorPercentages(n, outgoing, incoming, wasInfected, behaviorPercent);

        for (int i = 0; i < n; ++i) indices[i] = i;
        mergeSortIndicesDescending(behaviorPercent, indices, 0, n - 1);


        cout << "\n======= Behavior Suspicion Percentage =======\n";
        for (int k = 0; k < n; ++k) {
            int i = indices[k];
            cout << names[i] << " : " << (int)(behaviorPercent[i] + 0.5f) << "%";
            if (behaviorPercent[i] >= 75) cout << "  (HIGH)";
            else if (behaviorPercent[i] >= 40) cout << "  (MEDIUM)";
            else cout << "  (LOW)";
            cout << "\n";
        }
        cout << "=============================================\n";
    }

    void attackPrediction(NetworkGraph& graph, string startDevice) {
        string names[MAX_DEVICES];
        int outgoing[MAX_DEVICES], incoming[MAX_DEVICES], wasInfected[MAX_DEVICES];
        float behaviorPercent[MAX_DEVICES], predictionPercent[MAX_DEVICES];
        int indices[MAX_DEVICES];

        int n = buildDeviceStats(graph, names, outgoing, incoming, wasInfected);
        if (n == 0) {
            cout << "No devices available.\n";
            return;
        }

        computeBehaviorPercentages(n, outgoing, incoming, wasInfected, behaviorPercent);

        float rawScore[MAX_DEVICES];
        float maxRaw = 0.0f;

        for (int i = 0; i < n; ++i){
            int degree = outgoing[i] + incoming[i];
            bool direct = graph.hasDirectConnection(startDevice, names[i]);
            float score = 0.7f * behaviorPercent[i] + degree * 5.0f + (direct ? 20.0f : 0.0f);

            Device* d = graph.findDevice(names[i]);
            if (d && d->quarantined) score = 0.0f;

            rawScore[i] = score;
            if (score > maxRaw) maxRaw = score;
        }

        if (maxRaw <= 0.0f) {
            for (int i = 0; i < n; ++i) predictionPercent[i] = 0.0f;
        } else {
            for (int i = 0; i < n; ++i) {
                predictionPercent[i] = (rawScore[i] / maxRaw) * 100.0f;
            }
        }

        for (int i = 0; i < n; ++i) indices[i] = i;
        mergeSortIndicesDescending(behaviorPercent, indices, 0, n - 1);

        cout << "\n========= Attack Prediction for start = " << startDevice << " =========\n";
        for (int k = 0; k < n; ++k) {
            int i = indices[k];
            if (names[i] == startDevice) continue;
            cout << names[i] << " : " << (int)(predictionPercent[i] + 0.5f) << "%\n";
        }
        cout << "=====================================\n";
    }
};

#endif