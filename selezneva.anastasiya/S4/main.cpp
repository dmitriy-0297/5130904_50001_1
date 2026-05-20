#include "Graph.h"
#include "IoFmtGuard.h"
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <map>
#include <algorithm>
#include <iterator>
#include <locale.h>

const std::string ERROR_NOT_A_NUMBER = "Ошибка: Ожидалось целое число";
const std::string ERROR_NEGATIVE_VALUE = "Ошибка: Число не может быть отрицательным";
const std::string ERROR_MISSING_TYPE = "Ошибка: Укажите тип (DIRECTED/UNDIRECTED)";
const std::string ERROR_UNKNOWN_CMD = "Неизвестная команда";

Graph g_graph;

namespace selezneva {

    int readInt(std::istringstream& iss);
    void printVector(const std::vector<int>& v);

    void createGraph(std::istringstream& args);
    void addVertex(std::istringstream& args);
    void removeVertex(std::istringstream& args);
    void addEdge(std::istringstream& args);
    void removeEdge(std::istringstream& args);
    void dfs(std::istringstream& args);
    void topoSort(std::istringstream& args);
    void print(std::istringstream& args);

    void runApplication();

}

int main() {

    setlocale(LC_ALL, "Russian");

    selezneva::runApplication();

    return EXIT_SUCCESS;
}

namespace selezneva {

    int readInt(std::istringstream& iss) {
        int val = 0;
        if (!(iss >> val)) {
            throw std::invalid_argument(ERROR_NOT_A_NUMBER);
        }
        if (iss.peek() == '.') {
            throw std::invalid_argument(ERROR_NOT_A_NUMBER);
        }
        if (val < 0) {
            throw std::invalid_argument(ERROR_NEGATIVE_VALUE);
        }
        return val;
    }

    void printVector(const std::vector<int>& v) {
        IoFmtGuard guard(std::cout);
        if (!v.empty()) {
            std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(std::cout, " -> "));
            std::cout << v.back();
        }
        std::cout << "\n";
    }

    void createGraph(std::istringstream& args) {
        int count = readInt(args);
        std::string type;
        if (!(args >> type)) throw std::invalid_argument(ERROR_MISSING_TYPE);

        std::transform(type.begin(), type.end(), type.begin(), ::toupper);

        bool oriented = false;
        if (type == "DIRECTED" || type == "ORIENTED") {
            oriented = true;
        }
        else if (type == "UNDIRECTED" || type == "UNORIENTED") {
            oriented = false;
        }
        else {
            throw std::invalid_argument(ERROR_MISSING_TYPE);
        }

        g_graph = Graph(count, oriented);
        std::cout << "Граф создан. Вершин: " << count << ", тип: ";
        std::cout << (oriented ? "ориент." : "неориент.") << "\n";
    }

    void addVertex(std::istringstream&) {
        int v = g_graph.addVertex();
        std::cout << "Добавлена вершина " << v << "\n";
    }

    void removeVertex(std::istringstream& args) {
        int v = readInt(args);
        g_graph.removeVertex(v);
        std::cout << "Вершина " << v << " удалена\n";
    }

    void addEdge(std::istringstream& args) {
        int u = readInt(args);
        int v = readInt(args);
        int weight = 1;
        args >> weight;

        g_graph.addEdge(u, v, weight);
        std::cout << "Ребро (" << u << " -> " << v << ") с весом " << weight << " добавлено\n";
    }

    void removeEdge(std::istringstream& args) {
        int u = readInt(args);
        int v = readInt(args);
        int w = g_graph.removeEdge(u, v);
        std::cout << "Ребро (" << u << " -> " << v << ") с весом " << w << " удалено\n";
    }
    void dfs(std::istringstream& args) {
        int start = readInt(args);
        std::cout << "Обход DFS: ";
        printVector(g_graph.dfs(start));
    }

    void topoSort(std::istringstream&) {
        std::cout << "Топологическая сортировка: ";
        printVector(g_graph.topologicalSort());
    }

    void print(std::istringstream&) {
        std::cout << g_graph;
    }
    void runApplication(){
        std::cout << "ИНТЕРФЕЙС УПРАВЛЕНИЯ СТРУКТУРОЙ ДАННЫХ \"ГРАФ\"\n\n"
            << "  Программа принимает команды из стандартного потока ввода.\n"
            << "  Регистр символов не важен (команды 'create' и 'CREATE' эквивалентны).\n"
            << "  Для завершения работы программы отправьте сигнал EOF:\n\n"
            << "Список доступных команд и формат их ввода:\n"
            << "  1. CREATE <вершины> <тип>  - Создать граф (тип: DIRECTED / UNDIRECTED)\n"
            << "  2. ADD_VERTEX              - Добавить одну новую вершину\n"
            << "  3. REMOVE_VERTEX <вершина> - Удалить вершину и её рёбра\n"
            << "  4. ADD_EDGE <u> <v> [вес]  - Добавить/обновить ребро (вес по умолч. 1)\n"
            << "  5. REMOVE_EDGE <u> <v>     - Удалить ребро между u и v\n"
            << "  6. DFS <старт>             - Обход в глубину со стартовой вершины\n"
            << "  7. TOPO_SORT               - Топологическая сортировка (для ориентированных)\n"
            << "  8. PRINT                   - Отобразить списки смежности графа\n\n"
            << "Введите вашу команду, например \"CREATE 2 DIRECTED\": \n";

        std::map<std::string, std::function<void(std::istringstream&)>> cmdMap = {
            {"ADD_EDGE",      selezneva::addEdge},
            {"ADD_VERTEX",    selezneva::addVertex},
            {"DFS",           selezneva::dfs},
            {"CREATE",        selezneva::createGraph},
            {"PRINT",         selezneva::print},
            {"REMOVE_EDGE",   selezneva::removeEdge},
            {"REMOVE_VERTEX", selezneva::removeVertex},
            {"TOPO_SORT",     selezneva::topoSort}
        };

        std::string line = "";
        while (std::getline(std::cin, line)) {
            std::istringstream iss(line);
            std::string cmd;
            if (!(iss >> cmd)) {
                continue;
            }

            std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

            auto it = cmdMap.find(cmd);
            if (it != cmdMap.end()) {
                try {
                    it->second(iss);
                }
                catch (const std::exception& e) {
                    std::cerr << e.what() << "\n";
                }
            }
            else {
                std::cerr << ERROR_UNKNOWN_CMD << "\n";
            }
        }
    }
}
