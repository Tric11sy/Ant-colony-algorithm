#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    auto random_graph = ant::random_graph(100, 50);

    random_graph.print();



    return 0;
}