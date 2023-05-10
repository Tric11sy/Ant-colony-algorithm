#include <iostream>

#include <list>
#include <map>

#include <algorithm>
#include <vector>

//Для использования псевдо-рандома
#include <cstdlib>
#include <ctime>

//Количество муравьев
constexpr int ant_count = 10;

//Параметры для алгоритма

//Скорость испарения
constexpr double RHO = 0.5;

//Обновение феромона
double update_phero(const double old_phero) {
    double new_phero = 1.0 + old_phero;

    return new_phero;
}

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

//Граф(TO DO: Добавить метод изменения феромонов)
class Graph {
   public:
    //Конструктор по умолчанию
    Graph(int new_count) : edge_count(new_count){};

    //Добавление вершины в граф
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
                          << (*list_iter).length;
            }
            std::cout << std::endl;
        }
    }

    //Обновление феромонов(вершины для определения нужного ребра)
    void change_phero(int src, int dest) {
        std::cout << "Src:   " << src << std::endl;
        std::cout << "Dest:   " << dest << std::endl;

        // src - индекс в карте, dest - вершина в списке

        //Поиск нужной вершины
        auto iter = std::find_if(
            list_map[src].begin(),
            list_map[src].end(),
            [&](const Edge& edge) -> bool { return edge.dest_edge == dest; });

        std::cout << "  " << (*iter).dest_edge << ":" << (*iter).length;
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
    Ant(int new_current) {
        current = new_current;
        path_length = -1;
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

    //Добавление вершины в путь
    void add_path(int new_elem) { path.push_back(new_elem); }

    //Добавление вершины в список табу
    void add_tabu(int new_tabu) { tabu.push_back(new_tabu); }

   private:
    //Текущая вершина
    int current;
    //Список табу
    std::vector<int> tabu;
    //Пусть
    std::vector<int> path;
    //Длина пути
    int path_length;
};

//Муравьиный алгоритм
void AntColony() {
    //Создание графа
    auto graph = random_graph(30, 50);

    graph.print();

    //Инициализация муравьев
    std::vector<Ant> ants;
    for (int index = 0; index < ant_count; index++) {
        //Создание муравья(по одному в вершине)???
        //Важно: добавить проверку для размера графа
        Ant new_ant(index);

        //Добавление текущей вершины в путь
        new_ant.add_path(index);
        //Добавление текущей вершины в список табу
        new_ant.add_tabu(index);

        //Добавление в вектор муравьев
        ants.push_back(new_ant);
    }

    /*
            //Вывод списка табу
            for(auto item : ants)
            {
                item.print_tabu();
            }

            //Вывод пути
            for(auto item : ants)
            {
                item.print_path();
            }
    */
}

}  // namespace ant
