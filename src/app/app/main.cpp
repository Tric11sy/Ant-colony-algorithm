#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    // ant::AntColony();

    //Создание графа
    auto graph = ant::random_graph(10, 100);

    graph.print();
    std::cout << std::endl;

    graph.change_phero(1, 9);

    return 0;
}