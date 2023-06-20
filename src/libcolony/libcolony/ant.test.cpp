#include <gtest/gtest.h>
#include <libcolony/ant.hpp>

#include <vector>

void check_graph(
    ant::Graph& graph,
    std::vector<int>& cost,
    const double phero) {
    for (int index = 0; index < graph.get_count(); index++) {
        EXPECT_EQ(graph.get_length(index, index + 1), cost.at(index));
        EXPECT_EQ(graph.get_length(index + 1, index), cost.at(index));

        EXPECT_EQ(graph.get_phero(index, index + 1), phero);
        EXPECT_EQ(graph.get_phero(index + 1, index), phero);
    }
}

TEST(TestGRAPH, AddEdge) {
    ant::Graph test_graph(10);

    std::vector<int> cost = {178, 14, 29, 17, 28, 95, 999, 71, 1, 281};

    for (int index = 0; index < 10; index++) {
        test_graph.add_edge(index, index + 1, cost.at(index));
    }
    const double expected_phero = 0.1;

    check_graph(test_graph, cost, expected_phero);
}

void check_phero(ant::Graph& graph, const double phero) {
    for (int index = 0; index < graph.get_count(); index++) {
        EXPECT_EQ(graph.get_phero(index, index + 1), phero);
    }
}

TEST(TestGRAPH, Evaporation) {
    ant::Graph test_graph(10);

    std::vector<int> cost = {178, 14, 29, 17, 28, 95, 999, 71, 1, 281};

    for (int index = 0; index < 10; index++) {
        test_graph.add_edge(index, index + 1, cost.at(index));
    }

    double phero = 0.1;

    for (int index = 0; index < 10; index++) {
        check_phero(test_graph, phero);
        test_graph.phero_evaporation();
        phero *= RHO;
    }
}

TEST(TestColony, Results) {
    std::string filename = "test.txt";
    ant::random_graph(10, 100, filename);
    auto graph = ant::file_graph(filename);

    //Значения лучшего пути для прошлой итерации и текущей
    auto last_best_path = INT_MAX;
    auto current_best_path = INT_MAX;

    current_best_path = ant::AntColony(graph, 1);
    ASSERT_TRUE(current_best_path <= last_best_path);

    current_best_path = ant::AntColony(graph, 10);
    ASSERT_TRUE(current_best_path <= last_best_path);

    current_best_path = ant::AntColony(graph, 100);
    ASSERT_TRUE(current_best_path <= last_best_path);

    current_best_path = ant::AntColony(graph, 1000);
    ASSERT_TRUE(current_best_path <= last_best_path);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
