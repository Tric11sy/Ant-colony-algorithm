#include <iostream>

#include <list>
#include <map>

#include <algorithm>
#include <vector>

//Для использования псевдо-рандома
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <assert.h>

//Количество муравьев
constexpr int ant_count = 10;

//Параметры для алгоритма

//Скорость испарения
constexpr double RHO = 0.5;

//Параметры для определения вероятности
constexpr double ALPHA = 1.0;
constexpr double BETA = 5.0;

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

    //Добавление вершины в граф(3???)
    void add_edge(int src, int dest, int cost) {
        //Добавление ребра dest - src
        list_map[src].push_back(Edge(dest, cost, 1.0));
        //Добавление ребра src - dest
        list_map[dest].push_back(Edge(src, cost, 1.0));
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
                          << (*list_iter).length;
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
        (*iter).phero = (1 - RHO) * (*iter).phero + d_phero;
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
Graph random_graph(const int vertices, const int desnsity) {
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
            //Увеличение счетчика
            count++;
        }
    }
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

                //Рулетка
                double roll = ((double)rand() / RAND_MAX);

                //Удачный ролл
                if (roll < chance) {
                    // std::cout << "Winner:   " << dest << std::endl;
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
    //Пусть
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

//Муравьиный алгоритм
void AntColony() {
    //Создание графа
    auto graph = random_graph(10, 100);

    graph.print();
    std::cout << std::endl;

    std::cout << graph.get_phero(1, 2) << std::endl;

    //Инициализация муравьев
    std::vector<Ant> ants;
    //Инициализация муравьев
    init_ants(ants, graph.get_count());
    //Построенние путей муравьями
    ants_travelse(ants, graph);

    std::cout << "                  Tabu:" << std::endl;
    for (auto item : ants) {
        item.print_tabu();
    }
    std::cout << std::endl;

    std::cout << "                  Path:" << std::endl;
    for (auto item : ants) {
        item.print_path();
    }
    std::cout << std::endl;

    std::cout << "                  Length:" << std::endl;
    for (auto item : ants) {
        std::cout << item.get_path_length() << std::endl;
    }
    std::cout << std::endl;
}

}  // namespace ant
