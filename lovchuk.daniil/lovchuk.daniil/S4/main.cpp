#include "graph_tp.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

Graph g;
int vertexCount = 0;

const string ERROR_PREFIX = "ОШИБКА:";

const string ERR_INVALID_VERTEX = ERROR_PREFIX + " Некорректный индекс вершины";
const string ERR_INVALID_NUMBER = ERROR_PREFIX + " Ожидается целое число";
const string ERR_UNKNOWN_CMD = ERROR_PREFIX + " Неизвестная команда";
const string ERR_NO_PATH = ERROR_PREFIX + " Пути между вершинами не существует";
const string ERR_EMPTY_GRAPH = ERROR_PREFIX + " Граф пуст, операция невозможна";

void processCommand(const string& line);
void readCommands();

void cmdAddVertex(const string& args);
void cmdAddEdge(const string& args);
void cmdRemoveEdge(const string& args);
void cmdBFS(const string& args);
void cmdFindRoute(const string& args);
void cmdMaxDepth(const string& args);
void cmdShow();
void cmdReset();
void cmdCommands();

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "-----------------------------------------------------------\n";
    cout << "  Вариант 2.2: Обход в ширину неориентированного графа\n";
    cout << "-----------------------------------------------------------\n\n";

    cmdCommands();
    cout << "Введите команды (Ctrl+D / Ctrl+Z для завершения):\n";

    readCommands();

    return EXIT_SUCCESS;
}

void readCommands() {
    string line;

    if (!getline(cin, line)) {
        cout << "\n[EOF] Завершение работы программы...\n";
        return;
    }

    if (line.empty()) {
        readCommands();
        return;
    }

    processCommand(line);
    readCommands();
}

void processCommand(const string& line) {
    istringstream iss(line);
    string cmd;
    iss >> cmd;

    string args;
    getline(iss, args);
    if (!args.empty() && args[0] == ' ') args = args.substr(1);

    try {
        if (cmd == "add_vertex")   cmdAddVertex(args);
        else if (cmd == "add_edge")      cmdAddEdge(args);
        else if (cmd == "remove_edge")   cmdRemoveEdge(args);
        else if (cmd == "bfs")           cmdBFS(args);
        else if (cmd == "find_route")    cmdFindRoute(args);
        else if (cmd == "max_depth")     cmdMaxDepth(args);
        else if (cmd == "show")          cmdShow();
        else if (cmd == "reset")         cmdReset();
        else if (cmd == "commands")      cmdCommands();
        else if (cmd == "exit") {
            cout << "Выход из программы.\n";
            return;
        }
        else if (!cmd.empty()) {
            cerr << ERR_UNKNOWN_CMD << " " << cmd << "\n";
            cerr << "Введите 'commands' для просмотра списка доступных команд\n";
        }
    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << " " << e.what() << "\n";
    }
}

void cmdAddVertex(const string& args) {
    try {
        int n = stoi(args);
        if (n < 0) throw invalid_argument("Отрицательное число");

        for (int i = 0; i < n; i++) {
            g.addVertex();
        }

        vertexCount += n;
        cout << "[OK] Добавлено " << n << " вершин. Всего: " << vertexCount << "\n";
    }
    catch (...) {
        cerr << ERR_INVALID_NUMBER << "\n";
    }
}

void cmdAddEdge(const string& args) {
    try {
        istringstream iss(args);
        int u, v;
        iss >> u >> v;
        if (iss.fail()) throw invalid_argument("Нужны две вершины");

        g.addEdge(u, v);
        cout << "[OK] Ребро (" << u << ", " << v << ") добавлено\n";
    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << " " << e.what() << "\n";
    }
}

void cmdRemoveEdge(const string& args) {
    try {
        istringstream iss(args);
        int u, v;
        iss >> u >> v;
        if (iss.fail()) throw invalid_argument("Нужны две вершины");

        g.removeEdge(u, v);
        cout << "[OK] Ребро (" << u << ", " << v << ") удалено\n";
    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << " " << e.what() << "\n";
    }
}

void cmdBFS(const string& args) {
    try {
        int start = stoi(args);
        g.printBFSTree(start);
    }
    catch (...) {
        cerr << ERR_INVALID_VERTEX << "\n";
    }
}

void cmdFindRoute(const string& args) {
    try {
        istringstream iss(args);
        int u, v;
        iss >> u >> v;
        if (iss.fail()) throw invalid_argument("Нужны две вершины");

        g.printShortestPath(u, v);
    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << " " << e.what() << "\n";
    }
}

void cmdMaxDepth(const string& args) {
    try {
        int start = stoi(args);
        g.printDiameter(start);
    }
    catch (...) {
        cerr << ERR_INVALID_VERTEX << "\n";
    }
}

void cmdShow() {
    if (vertexCount == 0) cerr << ERR_EMPTY_GRAPH << "\n";
    else g.printGraph();
}

void cmdReset() {
    g.clear();
    vertexCount = 0;
    cout << "[OK] Граф полностью очищен\n";
}

void cmdCommands() {
    cout << "\n=== ДОСТУПНЫЕ КОМАНДЫ ===\n";
    cout << "  add_vertex <n>          - Добавить n вершин\n";
    cout << "  add_edge <u> <v>        - Добавить ребро между u и v\n";
    cout << "  remove_edge <u> <v>     - Удалить ребро между u и v\n";
    cout << "  bfs <start>             - Обход в ширину от вершины start\n";
    cout << "  find_route <u> <v>      - Кратчайший путь от u до v\n";
    cout << "  max_depth <start>       - Максимальная глубина дерева от start\n";
    cout << "  show                    - Показать структуру графа\n";
    cout << "  reset                   - Очистить граф\n";
    cout << "  commands                - Показать этот список\n";
    cout << "  exit                    - Выход из программы\n";
    cout << "\n[Подсказка] Ctrl+Z (Windows) или Ctrl+D (Linux) для завершения\n\n";
}
