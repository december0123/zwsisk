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

int main(int argc, char **argv)
{
//    TravellingSalesmanProblem tsp{"/home/dec/studia/sem6/zwsisk/graph.txt"};
    TravellingSalesmanProblem tsp(10, 1, 20);
    std::cout << "Genetyczny srednio zajmuje " << measureAverageTime<Milli, Solution>(50,
            std::bind(&TravellingSalesmanProblem::genetic, &tsp, 10, 1, 100)) << "ms" << std::endl;
//    std::cout << "Przeglad zupelny srednio zajmuje " << measureAverageTime<Milli, Solution>(50,
//            std::bind(&TravellingSalesmanProblem::bruteForce, &tsp)) << "ms" << std::endl;
//    std::cout << s.cost_ << std::endl;
//    for (auto& i : s.route_)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl;
//    s = tsp.bruteForce();
//    std::cout << s.cost_ << std::endl;
//    for (auto& i : s.route_)
//    {
//        std::cout << i << " ";
//    }

//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
