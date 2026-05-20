#include "graph.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "wave_algorithm.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <memory>

void printHelp();
void printDistances(const std::map<int, double>& distances, int source);
void printPath(const std::vector<int>& path);
void processCommandsRecursive(std::unique_ptr<DirectedWeightedGraph>& graph, std::istream& in);

void printHelp() {
    std::cout << "\n=== Commands ===\n"
              << "  1. ADD_NODE <node>                - add vertex\n"
              << "  2. ADD_EDGE <from> <to> <weight>  - add directed edge\n"
              << "  3. REMOVE_NODE <node>             - remove vertex\n"
              << "  4. REMOVE_EDGE <from> <to>        - remove edge\n"
              << "  5. DIJKSTRA <source>              - Dijkstra's algorithm\n"
              << "  6. BELLMAN_FORD <source>          - Bellman-Ford algorithm\n"
              << "  7. WAVE <source> <target>         - wave algorithm (BFS)\n"
              << "  8. PRINT                          - show graph\n"
              << "  9. HELP                           - show this help\n"
              << "  10. EXIT or EOF (Ctrl+D/Ctrl+Z)   - quit program\n"
              << "================================\n"
              << "Enter command (number or text): ";
}

void printDistances(const std::map<int, double>& distances, int source) {
    std::cout << "Shortest distances from " << source << ":\n";
    for (const auto& [node, distance] : distances) {
        std::cout << "  To " << node << ": ";
        if (distance == std::numeric_limits<double>::infinity()) {
            std::cout << "unreachable";
        } else {
            std::cout << std::fixed << std::setprecision(2) << distance;
        }
        std::cout << "\n";
    }
}

void printPath(const std::vector<int>& path) {
    if (path.empty()) {
        std::cout << "No path found\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";
}

std::string convertCommand(const std::string& input) {
    if (!input.empty()) {
        bool isNumber = true;
        for (char c : input) {
            if (c < '0' || c > '9') {
                isNumber = false;
                break;
            }
        }

        if (isNumber) {
            int num = std::stoi(input);
            switch (num) {
                case 1: return "ADD_NODE";
                case 2: return "ADD_EDGE";
                case 3: return "REMOVE_NODE";
                case 4: return "REMOVE_EDGE";
                case 5: return "DIJKSTRA";
                case 6: return "BELLMAN_FORD";
                case 7: return "WAVE";
                case 8: return "PRINT";
                case 9: return "HELP";
                case 10: return "EXIT";
                default: return input;
            }
        }
    }
    return input;
}

void processCommandsRecursive(std::unique_ptr<DirectedWeightedGraph>& graph, std::istream& in) {
    std::string line;

    std::cout << "> ";
    if (!std::getline(in, line)) {
        std::cout << "\nEOF detected. Goodbye!\n";
        graph.reset();
        return;
    }

    if (line.empty()) {
        processCommandsRecursive(graph, in);
        return;
    }

    std::istringstream iss(line);
    std::string firstToken;
    iss >> firstToken;

    if (firstToken.empty()) {
        processCommandsRecursive(graph, in);
        return;
    }

    std::string cleanToken;
    for (char c : firstToken) {
        if (c >= 'A' && c <= 'Z') {
            cleanToken += c;
        } else if (c >= 'a' && c <= 'z') {
            cleanToken += toupper(c);
        } else if (c >= '0' && c <= '9') {
            cleanToken += c;
        }
    }

    if (cleanToken.empty()) {
        cleanToken = firstToken;
    }

    std::string cmd = convertCommand(cleanToken);

    for (char& c : cmd) {
        c = toupper(c);
    }

    try {
        if (cmd == "ADD_NODE") {
            int node;
            if (iss >> node) {
                try {
                    graph->addNode(node);
                    std::cout << "Node " << node << " added.\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: ADD_NODE <node>\n";
            }
        }
        else if (cmd == "ADD_EDGE") {
            int from, to;
            double weight;
            if (iss >> from >> to >> weight) {
                try {
                    if (!graph->hasNode(from)) {
                        graph->addNode(from);
                    }
                    if (!graph->hasNode(to)) {
                        graph->addNode(to);
                    }
                    graph->addEdge(from, to, weight);
                    std::cout << "Edge " << from << " -> " << to
                              << " (weight=" << weight << ") added.\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: ADD_EDGE <from> <to> <weight>\n";
            }
        }
        else if (cmd == "REMOVE_NODE") {
            int node;
            if (iss >> node) {
                try {
                    graph->removeNode(node);
                    std::cout << "Node " << node << " removed.\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: REMOVE_NODE <node>\n";
            }
        }
        else if (cmd == "REMOVE_EDGE") {
            int from, to;
            if (iss >> from >> to) {
                try {
                    double w = graph->removeEdge(from, to);
                    std::cout << "Edge " << from << " -> " << to
                              << " (weight=" << w << ") removed.\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: REMOVE_EDGE <from> <to>\n";
            }
        }
        else if (cmd == "DIJKSTRA") {
            int source;
            if (iss >> source) {
                try {
                    if (!graph->hasNode(source)) {
                        std::cout << "Source node not found.\n";
                    } else if (graph->isEmpty()) {
                        std::cout << "Graph is empty.\n";
                    } else {
                        auto [dist, pred] = dijkstraShortestPath(*graph, source);
                        printDistances(dist, source);
                    }
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: DIJKSTRA <source>\n";
            }
        }
        else if (cmd == "BELLMAN_FORD") {
            int source;
            if (iss >> source) {
                try {
                    if (!graph->hasNode(source)) {
                        std::cout << "Source node not found.\n";
                    } else if (graph->isEmpty()) {
                        std::cout << "Graph is empty.\n";
                    } else {
                        auto [dist, pred] = bellmanFordShortestPath(*graph, source);
                        printDistances(dist, source);
                    }
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: BELLMAN_FORD <source>\n";
            }
        }
        else if (cmd == "WAVE") {
            int source, target;
            if (iss >> source >> target) {
                try {
                    if (!graph->hasNode(source) || !graph->hasNode(target)) {
                        std::cout << "One or both nodes not found.\n";
                    } else if (graph->isEmpty()) {
                        std::cout << "Graph is empty.\n";
                    } else {
                        auto [pred, success] = waveShortestPath(*graph, source, target);
                        if (success) {
                            auto path = reconstructPath(*graph, pred, target);
                            printPath(path);
                        } else {
                            std::cout << "No path found from " << source
                                      << " to " << target << "\n";
                        }
                    }
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Usage: WAVE <source> <target>\n";
            }
        }
        else if (cmd == "PRINT") {
            try {
                graph->print();
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else if (cmd == "HELP") {
            printHelp();
        }
        else if (cmd == "EXIT" || cmd == "QUIT") {
            std::cout << "Goodbye!\n";
            graph.reset();
            return;
        }
        else if (!cmd.empty()) {
            std::cout << "Unknown command: " << cmd
                      << ". Type HELP for available commands.\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Critical error: " << e.what() << "\n";
    }

    processCommandsRecursive(graph, in);
}

int main() {
    std::unique_ptr<DirectedWeightedGraph> graph = std::make_unique<DirectedWeightedGraph>();

    std::cout << "Directed Weighted Graph - Shortest Path Algorithms\n";
    printHelp();

    try {
        processCommandsRecursive(graph, std::cin);
    }
    catch (const std::bad_alloc& e) {
        std::cout << "Memory allocation error: " << e.what() << "\n";
        graph.reset();
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << "\n";
        graph.reset();
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cout << "Unknown fatal error occurred.\n";
        graph.reset();
        return EXIT_FAILURE;
    }

    graph.reset();

    return EXIT_SUCCESS;
}
