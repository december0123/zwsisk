#include "UndirectedGraph.hpp"
#include "TravellingSalesmanProblem.hpp"

#include <chrono>
#include <iostream>
#include <gtest/gtest.h>
#include <functional>

using Clock = std::chrono::high_resolution_clock;
using Micro = std::micro;
using Milli = std::milli;
using Seconds = std::ratio<1>;

template<typename T, typename C>
long double measureAverageTime(const int numOfTests, const std::function<C()>& f)
{
    if (!numOfTests)
    {
        return 0.0;
    }
    auto start = Clock::now();
    for (int i = 0; i < numOfTests; ++i)
    {
        f();
    }
    auto end = Clock::now();
    return std::chrono::duration<long double, T>(end - start).count() / numOfTests;
}

long double measureQuality(const int numOfTests)
{
    TravellingSalesmanProblem tsp(9, 1, 200);
    long double ratio = 0.0;
    for (int i = 0; i < numOfTests; ++i)
    {
        ratio += static_cast<long double>(tsp.bruteForce().cost_)
                / static_cast<long double>(tsp.genetic(100, 0.5, 100).cost_);
    }

    return ratio / numOfTests;
}
int main(int argc, char **argv)
{
//    TravellingSalesmanProblem tsp{"/home/dec/studia/sem6/zwsisk/graph.txt"};
//    constexpr unsigned NUM_OF_TESTS = 50U;
//    TravellingSalesmanProblem tsp(9, 1, 200);
//    Solution s = tsp.genetic(100, 0.5, 100);
//    std::cout << "Genetyczny srednio zajmuje "
//            << measureAverageTime<Milli, Solution>(NUM_OF_TESTS,
//                    std::bind(&TravellingSalesmanProblem::genetic, &tsp, 10, 1, 100)) << "ms"
//            << std::endl;
//    std::cout << "Przeglad zupelny srednio zajmuje "
//            << measureAverageTime<Milli, Solution>(NUM_OF_TESTS,
//                    std::bind(&TravellingSalesmanProblem::bruteForce, &tsp)) << "ms" << std::endl;

//    std::cout << "Jakość rozwiązania: " << measureQuality(NUM_OF_TESTS);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
