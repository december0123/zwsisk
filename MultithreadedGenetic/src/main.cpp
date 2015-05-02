#include "UndirectedGraph.hpp"
#include "TravellingSalesmanProblem.hpp"

#include "ProjectUtilities.hpp"
#include <iostream>
#include <gtest/gtest.h>

using ProjectUtilities::measureAverageTime;
using ProjectUtilities::measureQuality;

int main(int argc, char **argv)
{
    constexpr unsigned NUM_OF_TESTS = 50U;
//    TravellingSalesmanProblem tsp{"/home/dec/studia/sem6/zwsisk/graph.txt"};

    TravellingSalesmanProblem tsp(10, 1, 200);

    auto timeGenetic = [&]()
    {
        std::stringstream ss;
        ss << "Genetyczny srednio zajmuje "
        << measureAverageTime<Milli, Solution>(NUM_OF_TESTS,
                std::bind(&TravellingSalesmanProblem::genetic, &tsp, 50, 0.5, 100)) << "ms"
        << std::endl;
        std::cout << ss.str();
    };

    auto timeBrute = [&]()
    {
        std::stringstream ss;
        ss << "Przeglad zupelny srednio zajmuje "
        << measureAverageTime<Milli, Solution>(NUM_OF_TESTS,
                std::bind(&TravellingSalesmanProblem::bruteForce, &tsp)) << "ms" << std::endl;
        std::cout << ss.str();

    };

    auto quality = [&]()
    {
        std::stringstream ss;
        ss << "Błąd względny: "
            << measureQuality<Solution>(NUM_OF_TESTS, std::bind(&TravellingSalesmanProblem::bruteForce, &tsp),
                    std::bind(&TravellingSalesmanProblem::genetic, &tsp, 200, 0.5, 100));
            std::cout << ss.str();
    };
//
    auto g = std::async(std::launch::async, timeGenetic);
    auto b = std::async(std::launch::async, timeBrute);
    g.get();
    b.get();
    quality();

//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
