#include <libcolony/ant.hpp>

int main() {
    // Debug
    setbuf(stdout, nullptr);

    ant::Graph new_graph(10);

    new_graph.add_edge(1, 14, 100);

    new_graph.print();

    return 0;
}