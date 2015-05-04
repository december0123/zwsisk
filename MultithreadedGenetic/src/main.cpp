#include "UndirectedGraph.hpp"
#include "TravellingSalesmanProblem.hpp"

#include "ProjectUtilities.hpp"
#include <iostream>
#include <gtest/gtest.h>

using ProjectUtilities::measureAverageTime;
using ProjectUtilities::measureAverageRelativeError;
using ProjectUtilities::measureAverageGeneticTime;
using ProjectUtilities::measureAverageBruteForceTime;
using ProjectUtilities::measureGeneticQuality;

int main(int argc, char **argv)
{
    constexpr unsigned NUM_OF_TESTS = 1;
    constexpr unsigned NUM_OF_CITIES = 1;
    constexpr unsigned MIN_COST = 1;
    constexpr unsigned MAX_COST = 500;
    std::cout << "Genetyczny dla " + std::to_string(NUM_OF_CITIES) + " miast: "
            << measureAverageGeneticTime(NUM_OF_TESTS, NUM_OF_CITIES, MIN_COST, MAX_COST) << "ms\n";

    std::cout << "Błąd względny: " << measureGeneticQuality(NUM_OF_TESTS, NUM_OF_CITIES, MIN_COST, MAX_COST)
            << std::endl;

//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
