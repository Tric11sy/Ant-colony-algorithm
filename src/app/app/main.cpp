#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    auto random_graph = ant::random_graph(100, 10);

    random_graph.print();

    return 0;
}