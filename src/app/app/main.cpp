#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    //Псевдо-рандом
    srand((unsigned)time(0));

    // std::string filename1 = "random_graph10.txt";
    // std::string filename2 = "random_graph20.txt";
    // std::string filename3 = "random_graph30.txt";
    // std::string filename4 = "random_graph40.txt";
    std::string filename5 = "random_graph50.txt";

    //Создание случайного графа
    // auto graph = ant::random_graph(10, 100);

    auto graph = ant::file_graph(filename5);

    ant::AntColony(graph, 1);

    ant::AntColony(graph, 10);

    ant::AntColony(graph, 100);

    ant::AntColony(graph, 1000);

    return 0;
}
