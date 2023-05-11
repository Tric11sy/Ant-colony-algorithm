#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    std::string filename = "random_graph.txt";

    //Создание случайного графа
    auto graph = ant::random_graph(10, 100);
    ant::AntColony(graph, 10);

    //Создание графа по содержимому файла
    auto graph1 = ant::file_graph(filename);
    ant::AntColony(graph1, 1000);

    return 0;
}