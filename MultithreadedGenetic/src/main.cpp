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

template<typename T>
long double measureAverageTime(const unsigned numOfTests, const std::function<Solution()>& f)
{
    if (!numOfTests)
    {
        return 0.0;
    }
    auto start = Clock::now();
    for (unsigned i = 0; i < numOfTests; ++i)
    {
        f();
    }
    auto end = Clock::now();
    return std::chrono::duration<long double, T>(end - start).count() / numOfTests;
}

int main(int argc, char **argv)
{
//    TravellingSalesmanProblem tsp(12, 1, 100);
//    std::cout << measureAverageTime<Seconds>(1, std::bind(&TravellingSalesmanProblem::bruteForce, &tsp))
//            << std::endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
