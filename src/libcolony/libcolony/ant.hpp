#include <iostream>

#include <list>
#include <map>

#include <algorithm>
#include <vector>

//Для использования псевдо-рандома
#include <cmath>
#include <cstdlib>
#include <ctime>

//Для работы с файлами
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <assert.h>
#include <climits>

//Количество муравьев
constexpr int ant_count = 10;

//Параметры для алгоритма

//Скорость испарения
constexpr double RHO = 0.5;

//Параметры для определения вероятности
constexpr double ALPHA = 1.0;
constexpr double BETA = 5.0;

//Параметр отвечающий за добавление феромонов
constexpr double QVAL = 1.0;

namespace ant {

void test_func();

//Ребро
struct Edge {
    //Конструктор инициализации
    Edge(int dest, int len, double new_phero)
        : dest_edge(dest), length(len), phero(new_phero){};

    //Вторая вершина
    int dest_edge;
    //Длина ребра
    int length;
    //Феромоны
    double phero;
};

//Граф
class Graph {
   public:
    //Конструктор по умолчанию
    Graph(int new_count) : edge_count(new_count){};

    //Добавление ребра в граф
    void add_edge(int src, int dest, int cost) {
        //Добавление ребра dest - src
        list_map[src].push_back(Edge(dest, cost, 1.0 / edge_count));
        //Добавление ребра src - dest
        list_map[dest].push_back(Edge(src, cost, 1.0 / edge_count));
    }

    //Печать графа
    void print() {
        //Обход карты
        for (int vertex = 0; vertex < edge_count; vertex++) {
            //Обход списка
            for (auto list_iter = list_map[vertex].begin();
                 list_iter != list_map[vertex].end();
                 list_iter++) {
                std::cout << "  " << (*list_iter).dest_edge << ":"
                          << (*list_iter).length << "*" << (*list_iter).phero;
            }
            std::cout << std::endl;
        }
    }

    //Обновление феромонов(вершины для определения нужного ребра)
    void change_phero(int src, int dest, double d_phero) {
        //Поиск нужной вершины
        auto iter = std::find_if(
            list_map[src].begin(),
            list_map[src].end(),
            [&](const Edge& edge) -> bool { return edge.dest_edge == dest; });

        //Изменение феромонов
        (*iter).phero += +d_phero;
    }

    //Испарение феромонов
    void phero_evaporation() {
        //Обход карты
        for (int vertex = 0; vertex < edge_count; vertex++) {
            //Обход списка
            for (auto list_iter = list_map[vertex].begin();
                 list_iter != list_map[vertex].end();
                 list_iter++) {
                //Испарение
                (*list_iter).phero *= (1.0 - RHO);

                //Проверка
                if ((*list_iter).phero < 0.0) {
                    (*list_iter).phero *= 1.0 / edge_count;
                }
            }
        }
    }

    //Получение количества вершин
    int get_count() const { return edge_count; }

    //Получение феромонов заданной вершины
    double get_phero(int src, int dest) {
        auto iter = std::find_if(
            list_map[src].begin(),
            list_map[src].end(),
            [&](const Edge& edge) -> bool { return edge.dest_edge == dest; });
        return (*iter).phero;
    }

    //Получение длины заданной вершины
    int get_length(int src, int dest) {
        auto iter = std::find_if(
            list_map[src].begin(),
            list_map[src].end(),
            [&](const Edge& edge) -> bool { return edge.dest_edge == dest; });
        return (*iter).length;
    }

   private:
    //Количество вершин(rename)
    int edge_count;
    //Карта для каждой вешины
    std::map<int, std::list<Edge>> list_map;
};

//Генерация случайного графа
Graph random_graph(
    const int vertices,
    const int desnsity,
    std::string& filename) {
    //Псевдо-рандом
    srand((unsigned)time(0));

    //Создание графа
    Graph new_graph(vertices);

    //Вычисление количества ребер
    long double edge_count = vertices * (vertices - 1) / 2;
    edge_count = edge_count * desnsity / 100.0;

    //Карта для проверки графа(существует ли же ребро)
    std::map<std::pair<int, int>, int> check_map;

    //Счетчик для генерации ребер
    long double count = 0;

    //Открытие файла
    std::ofstream file;

    file.open(filename);

    //Количество вершин
    file << vertices << "\n";

    //Генерация ребер
    while (count < edge_count) {
        //Парамерты ребра
        int src, dest, cost;

        //Генерация номеров вершин для ребра
        src = rand() % vertices;
        dest = rand() % vertices;

        //Проверка на src < dest
        if (src > dest) {
            //Свап
            std::swap(src, dest);
        }

        //Пара ребер
        std::pair<int, int> edge(src, dest);
        //Генерация веса ребра
        cost = (rand() % 1000) + 1;

        //Вставка в граф
        if (check_map.find(edge) == check_map.end() and src != dest) {
            //Вставка в карту для проверки
            check_map[edge] = 1;
            //Вставка ребра в граф
            new_graph.add_edge(src, dest, cost);
            //Запись в файл
            file << src << " " << dest << " " << cost << "\n";

            //Увеличение счетчика
            count++;
        }
    }
    //Закрытие файла
    file.close();

    return new_graph;
}

//Генерация графа по файлу
Graph file_graph(std::string& filename) {
    //Переменная для файла
    std::ifstream file;

    //Открытие файла
    file.open(filename);

    //Проверка открытия файла
    if (!file.is_open()) {
        Graph graph(0);
        return graph;
    }

    //Строка для считывания
    std::string buffer;

    //Считывание количества вершин
    std::getline(file, buffer);

    //Количество вершин
    int vertices = std::stoi(buffer);

    //Создание графа
    Graph new_graph(vertices);

    //Считывание вершин
    while (std::getline(file, buffer)) {
        //Строковый поток для парсинга строки
        std::stringstream buffer_stream;
        buffer_stream << buffer;

        //Информация о ребре графа
        int src, dest, cost;

        buffer_stream >> src;
        buffer_stream >> dest;
        buffer_stream >> cost;

        //Вставка ребра в граф
        new_graph.add_edge(src, dest, cost);
    }

    //Закрытие файла
    file.close();
    return new_graph;
}

//"Муравей"
class Ant {
   public:
    //Конструктор инициализации
    Ant(int new_current, int edge_count) {
        current = new_current;
        path_length = 0;
        path_index = 1;

        //Выделение памяти под path
        path.reserve(edge_count);

        //Инициализация вектора табу(для облегчения жизни)
        tabu.reserve(edge_count);
        tabu.resize(edge_count, 0);
    }

    //Вывод списка табу
    void print_tabu() {
        for (auto item : tabu) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    //Вывод пути
    void print_path() {
        for (auto item : path) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    //Получение длины пути
    int get_path_length() { return path_length; }

    //Получения количества посещенных вершин
    int get_path_index() { return path_index; }

    //Получение номера вершины
    int get_num_path(int index) { return path.at(index); }

    //Получение текущей вершины
    int get_current() { return current; }

    //Добавление вершины в путь
    void add_path(int new_elem) { path.push_back(new_elem); }

    //Добавление вершины в список табу
    void add_tabu(int index) { tabu.at(index) = 1; }

    //Изменение текущей вершины
    void change_current(int new_current) { current = new_current; }

    //Изменение длины пути
    void change_path_length(int add) { path_length += add; }

    //Изменение количества посещенных вершин
    void change_path_index() { path_index += 1; }

    //Сброс значений в изначальные
    void reset(int ant_index) {
        //Сброс текущей вершины
        current = ant_index;
        //Сброс количества посещенных вершин
        path_index = 1;

        //Сброс списка табу
        tabu.resize(0);
        //Заполнение списка табу нулями
        tabu.resize(tabu.capacity(), 0);
        //Добавление текущей вершины в список табу
        tabu.at(ant_index) = 1;

        //Сброс пути
        path.resize(1);
        //Добавление текущей вершины в путь
        path.at(0) = ant_index;

        //Сброс длины пути
        path_length = 0;
    }

    //Подсчет вероятности
    double ant_product(double phero, int dist) {
        return ((pow(phero, ALPHA)) * (pow((1.0 / dist), BETA)));
    }

    //Выбор следущей вершины
    int select_next(Graph& graph) {
        //Получение количества вершин
        auto vertex_count = graph.get_count();

        //Делитель для формулы
        double denom = 0.0;

        //Цикл по всем вершинам
        for (int index = 0; index < vertex_count; index++) {
            //Проверка на табу
            if (tabu.at(index) == 0) {
                denom += ant_product(
                    graph.get_phero(current, index),
                    graph.get_length(current, index));
            }
        }

        //Проверка делителя
        assert(denom != 0.0);

        //Вершина
        int dest = 0;

        //Выбор вершины
        do {
            //Зацикливание перебора вершин
            if (dest == vertex_count) {
                dest = 0;
            }

            //Проверка на табу
            if (tabu.at(dest) == 0) {
                //Получение вероятности
                double chance = ant_product(
                                    graph.get_phero(current, dest),
                                    graph.get_length(current, dest)) /
                                denom;

                double roll = ((double)rand() / RAND_MAX);

                if (roll < chance) {
                    break;
                }
            }
            dest++;

        } while (1);
        return dest;
    }

   private:
    //Текущая вершина
    int current;
    //Количество посещенных вершин
    int path_index;
    //Список табу
    std::vector<int> tabu;
    //Путь
    std::vector<int> path;
    //Длина пути
    int path_length;
};

//Инициализация муравьев
void init_ants(std::vector<Ant>& ants, int count) {
    for (int index = 0; index < count; index++) {
        //Создание муравья
        Ant new_ant(index, count);

        //Добавление текущей вершины в путь
        new_ant.add_path(index);
        //Добавление текущей вершины в список табу
        new_ant.add_tabu(index);

        //Добавление в вектор муравьев
        ants.push_back(new_ant);
    }
}

//Построенние путей муравьями
void ants_travelse(std::vector<Ant>& ants, Graph& graph) {
    //Цикл по количеству муравьев(1 муравей - 1 вершина)
    for (int ant_index = 0; ant_index < graph.get_count(); ant_index++) {
        //Цикл по количеству вершин
        for (int index = 0; index < graph.get_count(); index++) {
            //Проверка на количество посещенных городов(эта проверка нужна, если
            //количество муравьев не равно количеству вершин)
            if (ants.at(index).get_path_index() < graph.get_count()) {
                //Выбор следующей вершины
                int dest = ants.at(index).select_next(graph);

                //Добавление выбранной вершины в список табу
                ants.at(index).add_tabu(dest);

                //Добаление выбранной вершины в путь
                ants.at(index).add_path(dest);

                //Изменение количества посещенных вершин
                ants.at(index).change_path_index();

                //Изменение длины пути
                ants.at(index).change_path_length(
                    graph.get_length(ants.at(index).get_current(), dest));

                //Случай "завершения алгоритма"(посещены все вершины)
                if (ants.at(index).get_path_index() == graph.get_count()) {
                    //Изменение длины пути
                    ants.at(index).change_path_length(
                        graph.get_length(index, dest));
                }
                //Изменение текущей вершины
                ants.at(index).change_current(dest);
            }
        }
    }
}

//Добавление феромонов
void add_phero(std::vector<Ant>& ants, Graph& graph) {
    // Src и dest для обобщения
    int src = 0;
    int dest = 1;

    //Цикл по муравьям
    for (auto ant : ants) {
        //Цикл по количеству вершин
        for (int index = 0; index < graph.get_count(); index++) {
            //Выбор значений для src и dest
            if (index < graph.get_count() - 1) {
                src = index;
                dest = index + 1;
            } else {
                src = index;
                dest = 0;
            }
            //Изменение феромонов у ребра  src - dest
            graph.change_phero(
                ant.get_num_path(src),
                ant.get_num_path(dest),
                QVAL / ant.get_path_length());

            //Изменение феромонов у ребра  dest - src
            graph.change_phero(
                ant.get_num_path(dest),
                ant.get_num_path(src),
                QVAL / ant.get_path_length());
        }
    }
}

//Сброс всех муравьев / обновление лучшего пути
void ants_reset(std::vector<Ant>& ants, int count, int& best_path) {
    //Цикл по муравьям
    for (int index = 0; index < count; index++) {
        //Обновление лучшего пути
        if (ants.at(index).get_path_length() < best_path) {
            best_path = ants.at(index).get_path_length();
        }
        ants.at(index).reset(index);
    }
}

//Одна итерация алгоритма
void iteration(std::vector<Ant>& ants, Graph& graph, int& best_path) {
    //Построенние путей муравьями
    ants_travelse(ants, graph);

    //Испарение феромонов
    graph.phero_evaporation();

    //Добавление феромонов
    add_phero(ants, graph);

    //Сброс значений / обновление лучшего пути
    ants_reset(ants, graph.get_count(), best_path);
}

//Муравьиный алгоритм
int AntColony(Graph& graph, int iter_count) {
    std::vector<Ant> ants;

    //Инициализация муравьев
    init_ants(ants, graph.get_count());

    //Лучший путь
    int best_path = INT_MAX;

    // iter_count итераций алгоритма
    for (int index = 0; index < iter_count; index++) {
        //Итерация алгоритма
        iteration(ants, graph, best_path);
    }

    std::cout << "                  Best path:   " << best_path << std::endl;
    return best_path;
}

}  // namespace ant
