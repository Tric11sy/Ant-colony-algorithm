#include <iostream>

#include <list>
#include <map>

//Для использования псевдо-рандома
#include <cstdlib>
#include <ctime>

namespace ant {

void test_func();

/*
    Представление графа:
*/

//Ребро
struct Edge {
    //Конструктор инициализации
    Edge(int dest, int len) : dest_edge(dest), length(len){};

    int dest_edge;
    int length;
};

//Граф
class Graph {
   public:
    //Конструктор по умолчанию
    Graph(int new_count) : edge_count(new_count){};

    //Добавление вершины в граф
    void add_edge(int src, int dest, int cost) {
        //Создание ребра src - dest
        Edge* new_edge = new Edge(dest, cost);
        //Добавление ребра в карту
        list_map[src].push_back(new_edge);

        //Создание ребра dest - src
        Edge* new_edge2 = new Edge(src, cost);
        //Добавление ребра в карту
        list_map[dest].push_back(new_edge2);
    }

    //Печать графа
    void print() {
        for (int vertex = 0; vertex < edge_count; vertex++) {
            std::list<Edge*> tmp_list = list_map[vertex];
            for (auto iter = tmp_list.begin(); iter != tmp_list.end(); iter++) {
                std::cout << "  " << (*iter)->dest_edge << ":"
                          << (*iter)->length;
            }
            std::cout << std::endl;
        }
    }

   private:
    //Количество вершин(rename)
    int edge_count;
    //Карта для каждой вешины
    std::map<int, std::list<Edge*>> list_map;
};

//Генерация случайного графа
Graph random_graph(const int vertices, const int desnsity) {
    //Псевдо-рандом
    srand((unsigned) time(0));

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
    while (count < vertices) {
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

}  // namespace ant