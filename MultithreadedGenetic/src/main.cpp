#include "ProjectUtilities.hpp"
#include "TSP.hpp"
#include "UndirectedGraph.hpp"

#include <iostream>
#include <gtest/gtest.h>

using namespace ProjectUtilities;

int main(int argc, char **argv)
{
    constexpr unsigned NUM_OF_TESTS = 10;
    constexpr unsigned NUM_OF_CITIES = 9;
    constexpr unsigned MIN_COST = 1;
    constexpr unsigned MAX_COST = 200;
    constexpr unsigned POPULATION_SIZE = 20;
    constexpr long double MUTATION_PROBABILITY = 0.01;
    constexpr unsigned NUM_OF_GENERATIONS = 20;
//    const TSP tsp(NUM_OF_CITIES, MIN_COST, MAX_COST);
    const TSP tsp("/home/dec/studia/sem6/zwsisk/graph_lower.txt");
    Solution s = tsp.genetic(POPULATION_SIZE, MUTATION_PROBABILITY, NUM_OF_GENERATIONS);
    std::cout << "Koszt genetyczny: " << s.cost_ << std::endl;
    s = tsp.bruteForce();
    std::cout << "Koszt brute: " << s.cost_ << std::endl;
    s = tsp.genetic_multi(POPULATION_SIZE, MUTATION_PROBABILITY, NUM_OF_GENERATIONS);
    std::cout << "Koszt multi genetyczny: " << s.cost_ << std::endl;
    printContainer(s.route_);

    std::cout << "Sredni czas genetycznego dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageGeneticTime(NUM_OF_TESTS, tsp, POPULATION_SIZE, MUTATION_PROBABILITY,
                    NUM_OF_GENERATIONS) << "ms\n";

    std::cout << "Sredni czas multi genetycznego dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageGeneticTime_multi(NUM_OF_TESTS, tsp, POPULATION_SIZE,
                    MUTATION_PROBABILITY, NUM_OF_GENERATIONS) << "ms\n";
    std::cout << "Sredni czas brute dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageBruteForceTime(NUM_OF_TESTS, tsp) << "ms\n";
    std::cout << "Sredni blad genetycznego wzgledem bruteforca: "
            << measureGeneticErrorRelativeToBruteForce(NUM_OF_TESTS, tsp, POPULATION_SIZE,
                    MUTATION_PROBABILITY, NUM_OF_GENERATIONS) << std::endl;

    std::cout << "Sredni blad genetycznego wzgledem multi genetycznego: "
            << measureGeneticErrorRelativeToMulti(NUM_OF_TESTS, tsp, POPULATION_SIZE,
                    MUTATION_PROBABILITY, NUM_OF_GENERATIONS) << std::endl;

//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
