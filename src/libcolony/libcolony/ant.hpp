#include <iostream>

#include <list>
#include <map>

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

    //Добавление вершины в граф(TO DO: FIX IT!!!)
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

    //Создание случайного графа

   private:
    //Количество вершин
    int edge_count;
    //Карта для каждой вешины
    std::map<int, std::list<Edge*>> list_map;
};

}  // namespace ant