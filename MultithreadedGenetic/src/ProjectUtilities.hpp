#ifndef PROJECTUTILITIES_HPP_
#define PROJECTUTILITIES_HPP_

#include "TSP.hpp"

#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Micro = std::micro;
using Milli = std::milli;
using Seconds = std::ratio<1>;

static std::mutex m;

namespace ProjectUtilities
{

template<typename T>
void printContainer(const T& cont)
{
    for (auto& i : cont)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

template<typename T, typename C>
long double measureAverageTime(const unsigned numOfTests, const std::function<C()>& f)
{
    if (!numOfTests)
    {
        return 0.0;
    }
    unsigned times { 0 };
    for (unsigned i = 0; i < numOfTests; ++i)
    {
        auto start = Clock::now();
        f();
        auto end = Clock::now();
        times += std::chrono::duration<long double, T>(end - start).count();
    }

    return times / static_cast<long double>(numOfTests);
}

long double measureAverageGeneticTime(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageTime<Milli, Solution>(numOfTests,
            std::bind(&TSP::genetic, &tsp, populationSize, mutationProbability,
                    numberOfGenerations));
}

long double measureAverageBruteForceTime(const unsigned numOfTests, const TSP& tsp)
{
    return measureAverageTime<Milli, Solution>(numOfTests, std::bind(&TSP::bruteForce, &tsp));
}

template<typename T>
long double measureAverageRelativeError(const int numOfTests, const std::function<T()>& brute,
        const std::function<T()>& gen)
{
    unsigned numOfThreads = std::thread::hardware_concurrency();
    while (numOfTests % numOfThreads)
    {
        --numOfThreads;
    }

//    std::atomic<unsigned> sumOfGenCosts { 0U };
    unsigned sumOfGenCosts = 0U;
    auto mGen = [&]()
    {
        for (unsigned i = 0; i < numOfTests / numOfThreads; ++i)
        {
            std::lock_guard<std::mutex> l{m};
            sumOfGenCosts += gen().cost_;
        }
    };

    std::vector<std::thread> threads(numOfThreads);
    for (auto& t : threads)
    {
        t = std::thread { mGen };
    }

    const long double bf = brute().cost_;

    for (auto& t : threads)
    {
        t.join();
    }

    long double genCost { static_cast<long double>(sumOfGenCosts)
            / static_cast<long double>(numOfTests) };
    return std::abs((bf - genCost) / genCost);
}

/*
 * Launches BruteForce and Genetic to calculate relative error.
 * Remember that BruteForce is O(n!) so use it carefuly.
 */
long double measureGeneticQuality(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageRelativeError<Solution>(numOfTests, std::bind(&TSP::bruteForce, &tsp),
            std::bind(&TSP::genetic, &tsp, populationSize, mutationProbability,
                    numberOfGenerations));
}

}

#endif /* PROJECTUTILITIES_HPP_ */
