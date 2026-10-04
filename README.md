# Cyber Network Threat Analyzer

A graph-based cybersecurity analysis tool built in C++ for CS221 (Data Structures & Algorithms). It models a computer network as a custom linked-list graph and simulates how an infection (malware/attack) spreads across connected devices, with tools to contain, trace, and prioritize the response.

Everything — the graph, the queue used for traversal, and the linked lists for devices and connections — is implemented from scratch using core DSA concepts (singly linked lists, a custom queue, BFS, DFS, and merge sort), without relying on STL containers for the core data structures.

## What it does

- **Models a network as a graph** — devices are nodes, connections are edges, both stored in custom singly linked lists (`NetworkGraph.h`).
- **Simulates infection spread with BFS** — starting from an infected device, the simulator (`Simulator.h`) walks outward through connections using a **custom-built queue** (`CustomQueue.h`), marking devices as infected in propagation order.
- **Quarantines devices** — an infected device can be isolated, removing it from further spread and simulating network segmentation as a containment response.
- **Ranks devices by behavior/risk** — tracks an infection count per device and uses **merge sort** to rank devices by risk score, surfacing the highest-priority devices for a responder to act on first.
- **Finds "patient zero"** — `NetworkAnalyzer.h` uses **DFS reachability analysis** to trace infection back through the network and identify which device could be the original source.
- **Checks neighbor infection status** — lets an analyst query whether a specific device's direct connections are compromised, without needing to trace the whole graph.

## Why it's relevant

This was a semester project, but the behavior-based detection and attack-path analysis map directly onto real SOC/threat-response concepts: BFS propagation modeling mirrors how malware moves through a network, DFS-based "patient zero" tracing mirrors root-cause/attack-path analysis, and quarantine mirrors containment response.

## Project structure

```
CyberNetworkThreatAnalyzer/
├── src/
│   ├── main.cpp              # Menu-driven CLI entry point
│   ├── NetworkGraph.h        # Graph: devices (nodes) + connections (edges), custom linked lists
│   ├── Simulator.h           # BFS infection spread + merge-sort-based risk ranking
│   ├── NetworkAnalyzer.h     # DFS reachability analysis, patient-zero tracing, neighbor checks
│   └── CustomQueue.h         # Custom queue implementation used by the BFS simulator
├── docs/
│   └── Final_Report.pdf      # Full project report
├── .gitignore
└── README.md
```

## Build & run

**Requirements:** a C++ compiler supporting C++11 or later (g++ recommended).

```bash
g++ -o analyzer src/main.cpp
./analyzer        # Linux/macOS
analyzer.exe      # Windows
```

## Usage

Running the program opens a menu-driven CLI:

```
---------- Cyber Network Threat Analyzer (Modular) ----------
1.  Add Device (Node)
2.  Show Devices
3.  Add Connection (Edge)
4.  Show Connections (Adjacency List)
5.  Simulate Infection Spread (BFS)
6.  Quarantine Device
7.  Show Quarantined Devices
8.  Show Behavior Scores
9.  Attack Prediction Engine
10. Check Neighbor Infection Status
11. Locate Patient Zero (DFS Analysis)
12. Exit
```

A typical walkthrough: add a few devices (option 1), connect them (option 3), mark one as infected and run the BFS simulation (option 5) to watch infection propagate across the network, then try quarantine (option 6) and patient-zero tracing (option 11) to see containment and root-cause analysis in action.

> **Note:** the menu uses `system("cls")` to clear the screen, which is Windows-specific. On Linux/macOS the program still runs correctly — the screen just won't clear between menu refreshes.

## Notes

This repository contains the final, consolidated version of the project. Earlier iterative drafts and course-submission deliverables are not included here to keep the codebase focused on the finished implementation; the full writeup and design rationale are in `docs/Final_Report.pdf`.
