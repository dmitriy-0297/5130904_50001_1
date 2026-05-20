#include "Graph.h"
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <iterator>

using namespace std;

// Константы ошибок
const char* const ERR_INVALID_INPUT =
    "Ошибка: Некорректный ввод! Введите целое неотрицательное число";
const char* const ERR_NOT_A_NUMBER =
    "Ошибка: Некорректный ввод! Введите целое число (дробные не допускаются)";
const char* const ERR_NUMBER_TOO_LARGE =
    "Ошибка: Некорректный ввод! Число слишком велико";
const char* const ERR_OUT_OF_RANGE =
    "Ошибка: Некорректный ввод! Введите число из диапазона: ";
const char* const ERR_EMPTY_GRAPH =
    "Ошибка: Граф пуст!";
const char* const ERR_SELF_LOOP =
    "Ошибка: Петли запрещены!";
const char* const ERR_EDGE_EXISTS =
    "Ошибка: Ребро уже существует!";
const char* const ERR_EDGE_NOT_EXISTS =
    "Ошибка: Ребро не существует!";
const char* const ERR_NEGATIVE_VERTICES =
    "Ошибка: Количество вершин должно быть целым неотрицательным числом";
const char* const ERR_TOO_MANY_VERTICES =
    "Ошибка: Некорректный ввод! Введите число из диапазона: 0 - 1000";
const char* const ERR_USAGE =
    "Ошибка: Некорректный ввод! Введите HELP для списка команд.";

static const int MAX_VERTICES = 1000;

int parseVertex(const string& s);
void dispatchLine(const string& line);

void cmdInit(istringstream& args);
void cmdAddVertex(istringstream& args);
void cmdAddEdge(istringstream& args);
void cmdRemoveVertex(istringstream& args);
void cmdRemoveEdge(istringstream& args);
void cmdPrint(istringstream& args);
void cmdBFS(istringstream& args);
void cmdPath(istringstream& args);
void cmdDiameter(istringstream& args);
void cmdHelp(istringstream& args);

using Handler = function<void(istringstream&)>;

static const unordered_map<string, Handler> COMMANDS = {
    { "INIT",          cmdInit         },
    { "ADD_VERTEX",    cmdAddVertex    },
    { "ADD_EDGE",      cmdAddEdge      },
    { "REMOVE_VERTEX", cmdRemoveVertex },
    { "REMOVE_EDGE",   cmdRemoveEdge   },
    { "PRINT",         cmdPrint        },
    { "BFS",           cmdBFS          },
    { "PATH",          cmdPath         },
    { "DIAMETER",      cmdDiameter     },
    { "HELP",          cmdHelp         },
};

static Graph g_graph;

int main() {
    setlocale(LC_ALL, "rus");
    cout << "Обход в ширину неориентированного графа (вариант 2.2)\n";
    cout << "Введите HELP для списка команд. "
         << "Для выхода — EOF (Ctrl+D / Ctrl+Z+Enter).\n\n";

    string line;
    while (getline(cin, line))
        dispatchLine(line);

    cout << "\nРабота завершена.\n";
    return EXIT_SUCCESS;
}

int parseVertex(const string& s) {
    if (s.empty())
        throw invalid_argument(ERR_INVALID_INPUT);

    size_t pos = 0;
    int val;
    try {
        val = stoi(s, &pos);
    }
    catch (const invalid_argument&) {
        throw invalid_argument(ERR_NOT_A_NUMBER);
    }
    catch (const out_of_range&) {
        throw out_of_range(ERR_NUMBER_TOO_LARGE);
    }

    if (pos != s.size())
        throw invalid_argument(ERR_NOT_A_NUMBER);
    if (val < 0)
        throw invalid_argument(ERR_INVALID_INPUT);
    return val;
}

void dispatchLine(const string& line) {
    istringstream iss(line);
    string cmd;
    if (!(iss >> cmd)) return;

    transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    auto it = COMMANDS.find(cmd);
    if (it == COMMANDS.end()) {
        cerr << "Ошибка: Неизвестная команда \"" << cmd
             << "\". Введите HELP для списка команд.\n";
        return;
    }

    try {
        it->second(iss);
    }
    catch (const exception& e) {
        cerr << e.what() << "\n";
    }
}

void cmdInit(istringstream& args) {
    string line;
    if (!(args >> line))
        throw invalid_argument(ERR_USAGE);

    int n = parseVertex(line);
    if (n > MAX_VERTICES)
        throw out_of_range(ERR_TOO_MANY_VERTICES);
    g_graph = Graph(n);

    int maxEdges = n * (n - 1) / 2;
    cout << "Граф создан. Вершин: " << n;
    if (n > 0)
        cout << " (0.." << n - 1 << "), максимум рёбер: " << maxEdges;
    cout << "\n";
}

void cmdAddVertex(istringstream&) {
    g_graph.addVertex();
    int n = g_graph.getVertexCount();
    int maxEdges = n * (n - 1) / 2;
    cout << "Вершина " << n - 1 << " добавлена."
         << " Всего вершин: " << n
         << ", максимум рёбер: " << maxEdges << "\n";
}

void cmdAddEdge(istringstream& args) {
    string su, sv;
    if (!(args >> su >> sv))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int u = parseVertex(su);
    int v = parseVertex(sv);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(u) || !g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));
    if (u == v)
        throw invalid_argument(ERR_SELF_LOOP);
    if (g_graph.hasEdge(u, v))
        throw invalid_argument(ERR_EDGE_EXISTS);

    g_graph.addEdge(u, v);
    cout << "Ребро " << u << "-" << v << " добавлено\n";
}

void cmdRemoveVertex(istringstream& args) {
    string line;
    if (!(args >> line))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int v = parseVertex(line);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));

    g_graph.removeVertex(v);

    int newN = g_graph.getVertexCount();
    int maxEdges = newN * (newN - 1) / 2;
    cout << "Вершина " << v << " удалена."
         << " Всего вершин: " << newN;
    if (newN > 0)
        cout << ", максимум рёбер: " << maxEdges;
    cout << "\n";
}

void cmdRemoveEdge(istringstream& args) {
    string su, sv;
    if (!(args >> su >> sv))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int u = parseVertex(su);
    int v = parseVertex(sv);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(u) || !g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));
    if (!g_graph.hasEdge(u, v))
        throw logic_error(ERR_EDGE_NOT_EXISTS);

    g_graph.removeEdge(u, v);
    cout << "Ребро " << u << "-" << v << " удалено\n";
}

void cmdPrint(istringstream&) {
    g_graph.print();
}

void cmdBFS(istringstream& args) {
    string line;
    if (!(args >> line))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int v = parseVertex(line);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));

    g_graph.printBFSTree(v);
}

void cmdPath(istringstream& args) {
    string su, sv;
    if (!(args >> su >> sv))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int u = parseVertex(su);
    int v = parseVertex(sv);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(u) || !g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));

    g_graph.printShortestPath(u, v);
}

void cmdDiameter(istringstream& args) {
    string line;
    if (!(args >> line))
        throw invalid_argument(ERR_USAGE);

    if (g_graph.isEmpty())
        throw logic_error(ERR_EMPTY_GRAPH);

    int v = parseVertex(line);
    int n = g_graph.getVertexCount();

    if (!g_graph.hasVertex(v))
        throw out_of_range(string(ERR_OUT_OF_RANGE) + "0 - "
                           + to_string(n - 1));

    g_graph.printDiameterPath(v);
}

void cmdHelp(istringstream&) {
    cout << "\nДоступные команды:\n"
         << "  INIT <n>              — создать граф с n вершинами "
         << "(0 = пустой граф)\n"
         << "  ADD_VERTEX            — добавить новую вершину\n"
         << "  ADD_EDGE <u> <v>      — добавить ребро между вершинами u и v\n"
         << "  REMOVE_VERTEX <v>     — удалить вершину v\n"
         << "  REMOVE_EDGE <u> <v>   — удалить ребро u-v\n"
         << "  PRINT                 — вывести список смежности\n"
         << "  BFS <start>           — обход в ширину от вершины start\n"
         << "  PATH <start> <target> — кратчайший путь от start до target\n"
         << "  DIAMETER <start>      — диаметр BFS-дерева\n"
         << "  HELP                  — эта справка\n"
         << "  (EOF: Ctrl+D / Ctrl+Z+Enter для выхода)\n\n";
}
