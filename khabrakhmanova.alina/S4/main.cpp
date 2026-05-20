#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include "Graph.h"

bool isInteger(const std::string& str) {
    if (str.empty()) return false;
    
    size_t start = 0;
    if (str[0] == '-') {
        if (str.length() == 1) return false;
        start = 1;
    }
    
    for (size_t i = start; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

bool isPositiveInteger(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

void printHelp() {
    std::cout << "\nAvailable commands:" << std::endl;
    std::cout << "  create                     - create empty graph" << std::endl;
    std::cout << "  add_node [index]           - add vertex (without index - add to end)" << std::endl;
    std::cout << "  add_edge from to weight    - add edge" << std::endl;
    std::cout << "  remove_node node           - remove vertex" << std::endl;
    std::cout << "  remove_edge from to        - remove edge" << std::endl;
    std::cout << "  print                      - print graph" << std::endl;
    std::cout << "  kruskal                    - find MST using Kruskal's algorithm" << std::endl;
    std::cout << "  prim [start]               - find MST using Prim's algorithm" << std::endl;
    std::cout << "  connected                  - check if graph is connected" << std::endl;
    std::cout << "  tests                      - run unit tests" << std::endl;
    std::cout << "  help                       - show this help" << std::endl;
    std::cout << "  exit                       - exit program" << std::endl;
    std::cout << std::endl;
}

void handleCreate(Graph& graph) {
    graph.createEmpty();
    std::cout << "Empty graph created" << std::endl;
}

void handleAddNode(Graph& graph, const std::string& arg) {
    if (arg.empty()) {
        graph.addNode();
        std::cout << "Node " << graph.getVertexCount() - 1 << " added" << std::endl;
    } else if (isPositiveInteger(arg)) {
        int node = std::stoi(arg);
        try {
            graph.addNode(node);
            std::cout << "Node " << node << " added" << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else {
        std::cerr << "Error: Invalid node index" << std::endl;
    }
}

void handleAddEdge(Graph& graph, const std::string& args) {
    std::istringstream iss(args);
    int from, to, weight;
    if (iss >> from >> to >> weight) {
        std::string remaining;
        if (!(iss >> remaining)) {
            try {
                graph.addEdge(from, to, weight);
                std::cout << "Edge added successfully!" << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
            return;
        }
    }
    std::cerr << "Error: Invalid input. Usage: add_edge from to weight" << std::endl;
}

void handleRemoveNode(Graph& graph, const std::string& arg) {
    if (graph.isEmpty()) {
        std::cerr << ERROR_GRAPH_EMPTY << std::endl;
        return;
    }
    
    if (arg.empty() || !isPositiveInteger(arg)) {
        std::cerr << "Error: Invalid node index. Usage: remove_node node" << std::endl;
        return;
    }
    
    int node = std::stoi(arg);
    try {
        graph.removeNode(node);
        std::cout << "Node " << node << " removed successfully!" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void handleRemoveEdge(Graph& graph, const std::string& args) {
    if (graph.isEmpty()) {
        std::cerr << ERROR_GRAPH_EMPTY << std::endl;
        return;
    }
    
    std::istringstream iss(args);
    int from, to;
    if (iss >> from >> to) {
        std::string remaining;
        if (!(iss >> remaining)) {
            try {
                graph.removeEdge(from, to);
                std::cout << "Edge removed successfully!" << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
            return;
        }
    }
    std::cerr << "Error: Invalid input. Usage: remove_edge from to" << std::endl;
}

void handleKruskal(Graph& graph) {
    if (graph.isEmpty()) {
        std::cerr << ERROR_GRAPH_EMPTY << std::endl;
        return;
    }
    
    try {
        auto mst = graph.kruskalMST();
        
        if (mst.empty()) {
            std::cout << "MST is empty (graph has 0 or 1 vertices)" << std::endl;
            return;
        }
        
        std::cout << "\nMinimum Spanning Tree (Kruskal):" << std::endl;
        for (const auto& edge : mst) {
            std::cout << "  " << edge.from << " --(" << edge.weight << ")-- " << edge.to << std::endl;
        }
        std::cout << "Total MST weight: " << graph.getTotalWeight(mst) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void handlePrim(Graph& graph, const std::string& arg) {
    if (graph.isEmpty()) {
        std::cerr << ERROR_GRAPH_EMPTY << std::endl;
        return;
    }
    
    int start = 0;
    if (!arg.empty()) {
        if (!isPositiveInteger(arg)) {
            std::cerr << "Error: Invalid start vertex" << std::endl;
            return;
        }
        start = std::stoi(arg);
    }
    
    try {
        auto mst = graph.primMST(start);
        
        if (mst.empty()) {
            std::cout << "MST is empty (graph has 0 or 1 vertices)" << std::endl;
            return;
        }
        
        std::cout << "\nMinimum Spanning Tree (Prim, start=" << start << "):" << std::endl;
        for (const auto& edge : mst) {
            std::cout << "  " << edge.from << " --(" << edge.weight << ")-- " << edge.to << std::endl;
        }
        std::cout << "Total MST weight: " << graph.getTotalWeight(mst) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void handleConnected(Graph& graph) {
    if (graph.isEmpty()) {
        std::cout << "Graph is empty" << std::endl;
        return;
    }
    
    bool connected = graph.isConnected();
    if (connected) {
        std::cout << "Graph is connected" << std::endl;
    } else {
        std::cout << "Graph is NOT connected" << std::endl;
    }
}

void runTests() {
    std::cout << "\nRUNNING UNIT TESTS\n" << std::endl;
    
    // Test 1: Create empty graph
    std::cout << "Test 1: Create empty graph" << std::endl;
    Graph g1;
    if (g1.isEmpty() && g1.getVertexCount() == 0 && g1.getEdgeCount() == 0) {
        std::cout << "  PASS" << std::endl;
    } else {
        std::cout << "  FAIL" << std::endl;
    }
    
    // Test 2: Add nodes
    std::cout << "\nTest 2: Add nodes" << std::endl;
    Graph g2;
    g2.addNode();
    g2.addNode();
    g2.addNode();
    if (g2.getVertexCount() == 3 && !g2.isEmpty() && 
        g2.hasNode(0) && g2.hasNode(1) && g2.hasNode(2) && !g2.hasNode(3)) {
        std::cout << "  PASS" << std::endl;
    } else {
        std::cout << "  FAIL" << std::endl;
    }
    
    // Test 3: Add edges
    std::cout << "\nTest 3: Add edges" << std::endl;
    Graph g3;
    for (int i = 0; i < 4; ++i) g3.addNode();
    g3.addEdge(0, 1, 5);
    g3.addEdge(0, 2, 3);
    g3.addEdge(1, 3, 7);
    if (g3.getEdgeCount() == 3 && g3.hasEdge(0, 1) && g3.hasEdge(1, 0) &&
        g3.hasEdge(0, 2) && g3.hasEdge(2, 0) && g3.hasEdge(1, 3) && g3.hasEdge(3, 1) &&
        !g3.hasEdge(0, 3) && g3.getEdgeWeight(0, 1) == 5 && g3.getEdgeWeight(1, 0) == 5) {
        std::cout << "  PASS" << std::endl;
    } else {
        std::cout << "  FAIL" << std::endl;
    }
    
    // Test 4: Negative weight validation
    std::cout << "\nTest 4: Negative weight validation" << std::endl;
    Graph g4;
    g4.addNode();
    g4.addNode();
    bool caught = false;
    try {
        g4.addEdge(0, 1, -5);
    } catch (const std::runtime_error& e) {
        if (std::string(e.what()) == ERROR_NEGATIVE_WEIGHT) caught = true;
    }
    std::cout << (caught ? "  PASS" : "  FAIL") << std::endl;
    
    // Test 5: Kruskal's MST
    std::cout << "\nTest 5: Kruskal's MST algorithm" << std::endl;
    Graph g5;
    for (int i = 0; i < 4; ++i) g5.addNode();
    g5.addEdge(0, 1, 2);
    g5.addEdge(0, 2, 3);
    g5.addEdge(0, 3, 1);
    g5.addEdge(1, 2, 4);
    g5.addEdge(1, 3, 5);
    g5.addEdge(2, 3, 6);
    
    try {
        auto mst5 = g5.kruskalMST();
        int weight5 = g5.getTotalWeight(mst5);
        if (mst5.size() == 3 && weight5 == 6) {
            std::cout << "  PASS (MST weight = " << weight5 << ")" << std::endl;
        } else {
            std::cout << "  FAIL" << std::endl;
        }
    } catch (...) {
        std::cout << "  FAIL" << std::endl;
    }
    
    // Test 6: Prim's MST
    std::cout << "\nTest 6: Prim's MST algorithm" << std::endl;
    Graph g6;
    for (int i = 0; i < 4; ++i) g6.addNode();
    g6.addEdge(0, 1, 2);
    g6.addEdge(0, 2, 3);
    g6.addEdge(0, 3, 1);
    g6.addEdge(1, 2, 4);
    g6.addEdge(1, 3, 5);
    g6.addEdge(2, 3, 6);
    
    try {
        auto mst6 = g6.primMST(0);
        int weight6 = g6.getTotalWeight(mst6);
        if (mst6.size() == 3 && weight6 == 6) {
            std::cout << "  PASS (MST weight = " << weight6 << ")" << std::endl;
        } else {
            std::cout << "  FAIL" << std::endl;
        }
    } catch (...) {
        std::cout << "  FAIL" << std::endl;
    }
    
    // Test 7: Self-loop rejection
    std::cout << "\nTest 7: Self-loop rejection" << std::endl;
    Graph g7;
    g7.addNode();
    caught = false;
    try {
        g7.addEdge(0, 0, 5);
    } catch (const std::runtime_error& e) {
        if (std::string(e.what()) == ERROR_SELF_LOOP) caught = true;
    }
    std::cout << (caught ? "  PASS" : "  FAIL") << std::endl;
    
    std::cout << "\nALL TESTS COMPLETED\n" << std::endl;
}

int main() {
    Graph graph;
    std::string line;
    
    std::cout << "Minimum Spanning Tree of Undirected Graph" << std::endl;
    std::cout << "Algorithms: Kruskal & Prim" << std::endl;
    printHelp();
    
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\nEOF detected. Exiting program..." << std::endl;
            break;
        }
        
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        
        std::string args;
        std::getline(iss, args);
        if (!args.empty() && args[0] == ' ') {
            args = args.substr(1);
        }
        
        if (cmd == "exit") {
            std::cout << "Exiting program..." << std::endl;
            break;
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "create") {
            handleCreate(graph);
        } else if (cmd == "add_node") {
            handleAddNode(graph, args);
        } else if (cmd == "add_edge") {
            handleAddEdge(graph, args);
        } else if (cmd == "remove_node") {
            handleRemoveNode(graph, args);
        } else if (cmd == "remove_edge") {
            handleRemoveEdge(graph, args);
        } else if (cmd == "print") {
            graph.printGraph();
        } else if (cmd == "kruskal") {
            handleKruskal(graph);
        } else if (cmd == "prim") {
            handlePrim(graph, args);
        } else if (cmd == "connected") {
            handleConnected(graph);
        } else if (cmd == "tests") {
            runTests();
        } else {
            std::cerr << "Unknown command: " << cmd << ". Type help for available commands." << std::endl;
        }
    }
    
    return EXIT_SUCCES;
}
