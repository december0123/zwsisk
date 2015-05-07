#include "UndirectedGraph.hpp"
#include "ProjectUtilities.hpp"
#include <iostream>
#include <gtest/gtest.h>
#include "TSP.hpp"

using namespace ProjectUtilities;

int main(int argc, char **argv)
{
    constexpr unsigned NUM_OF_TESTS = 100;
    constexpr unsigned NUM_OF_CITIES = 2;
    constexpr unsigned MIN_COST = 1;
    constexpr unsigned MAX_COST = 50;
    constexpr unsigned POPULATION_SIZE = 50;
    constexpr long double MUTATION_PROBABILITY = 0.01;
    constexpr unsigned NUM_OF_GENERATIONS = 50;
    const TSP tsp(NUM_OF_CITIES, MIN_COST, MAX_COST);
//    Route r1{1, 0, 2, 3};
//    Route r2{2, 3, 0, 1};
//    printContainer(tsp.createOffspring(r1, r2));

    std::cout << "Genetyczny dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageGeneticTime(NUM_OF_TESTS, tsp, POPULATION_SIZE, MUTATION_PROBABILITY,
                    NUM_OF_GENERATIONS) << "ms\n";

    std::cout << "Brute dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageBruteForceTime(NUM_OF_TESTS, tsp) << "ms\n";

    std::cout << "Błąd względny: "
            << measureGeneticQuality(NUM_OF_TESTS, tsp, POPULATION_SIZE, MUTATION_PROBABILITY,
                    NUM_OF_GENERATIONS) << std::endl;

//    Solution s = tsp.genetic(POPULATION_SIZE, MUTATION_PROBABILITY,
//            NUM_OF_GENERATIONS);
//    std::cout << "\nGENETYCZNY\n" << s.cost_ << std::endl;
//
//    printContainer(s.route_);
//
//    s = tsp.bruteForce();
//    std::cout << "\nBrute\n" << s.cost_ << std::endl;
//
//    printContainer(s.route_);
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
