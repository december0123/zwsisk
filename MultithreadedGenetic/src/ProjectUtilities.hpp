#ifndef PROJECTUTILITIES_HPP_
#define PROJECTUTILITIES_HPP_

#include "TSP.hpp"

#include <atomic>
#include <chrono>
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

/*
 * Prints given container in a form of
 * c[0] c[1] c[2] ... c[n] \n
 */
template<typename T>
void printContainer(const T& cont)
{
    for (auto& i : cont)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// Measures average execution time of given function
template<typename T, typename Lambda>
long double measureAverageTime(const unsigned numOfTests, Lambda&& f)
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
    return static_cast<long double>(times) / static_cast<long double>(numOfTests);
}

// Measures average genetic execution time
long double measureAverageGeneticTime(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageTime<Milli>(numOfTests,
            [&](){return tsp.genetic(populationSize, mutationProbability, numberOfGenerations);}
    );
}

// Measures average genetic_multi execution time
long double measureAverageGeneticTime_multi(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageTime<Milli>(numOfTests,
            [&](){
                return tsp.genetic_multi(populationSize, mutationProbability, numberOfGenerations);
            }
    );
}

// Measures average bruteforce execution time
long double measureAverageBruteForceTime(const unsigned numOfTests, const TSP& tsp)
{
    return measureAverageTime<Milli>(numOfTests, [&](){return tsp.bruteForce();});
}

/*
 * Calculates error of funtion "f2" relative to function "f1".
 * Only function f2 is launched multiple times in parallel.
 * Function f1 is launched only once (created to measure error relative to brute force).
 */
template<typename Lambda, typename Lambda2>
long double measureAverageRelativeError(const int numOfTests, Lambda&& f1,
        Lambda2&& f2)
{
    unsigned numOfThreads = std::thread::hardware_concurrency();
    while (numOfTests % numOfThreads)
    {
        --numOfThreads;
    }

    std::atomic<unsigned> sumOfF2Costs { 0U };
    auto f2Tester = [&]()
    {
        for (unsigned i = 0; i < numOfTests / numOfThreads; ++i)
        {
            sumOfF2Costs += f2().cost_;
        }
    };

    std::vector<std::thread> threads(numOfThreads);
    for (auto& t : threads)
    {
        t = std::thread { f2Tester };
    }

    const long double f1Cost = f1().cost_;

    for (auto& t : threads)
    {
        t.join();
    }

    long double averageF2Cost { static_cast<long double>(sumOfF2Costs.load())
            / static_cast<long double>(numOfTests) };
    return std::abs((f1Cost - averageF2Cost)) / f1Cost;
}

/*
 * Both of them are launched multiple times in parallel
 * Calculates error of funtion "f2" relative to function "f1".
 */
template<typename Lambda, typename Lambda2>
long double measureAverageRelativeError_multi(const int numOfTests, Lambda&& f1,
        Lambda2&& f2)
{
    unsigned numOfThreads = std::thread::hardware_concurrency();
    while (numOfTests % numOfThreads)
    {
        --numOfThreads;
    }

    std::atomic<unsigned> sumOfF1Costs { 0U };
    std::atomic<unsigned> sumOfF2Costs { 0U };
    auto mGen = [&]()
    {
        for (unsigned i = 0; i < numOfTests / numOfThreads; ++i)
        {
            sumOfF1Costs += f1().cost_;
            sumOfF2Costs += f2().cost_;
        }
    };

    std::vector<std::thread> threads(numOfThreads);
    for (auto& t : threads)
    {
        t = std::thread { mGen };
    }

    for (auto& t : threads)
    {
        t.join();
    }

    const long double f1Cost { static_cast<long double>(sumOfF1Costs.load())
            / static_cast<long double>(numOfTests) };
    const long double f2Cost { static_cast<long double>(sumOfF2Costs.load())
            / static_cast<long double>(numOfTests) };
    return std::abs((f2Cost - f1Cost)) / f2Cost;
}

/*
 * Launches BruteForce and Genetic to calculate relative error.
 * Remember that BruteForce is O(n!) so use it carefuly.
 */
long double measureGeneticErrorRelativeToBruteForce(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageRelativeError(numOfTests, [&](){return tsp.bruteForce();},
            [&](){return tsp.genetic(populationSize, mutationProbability, numberOfGenerations);}
    );
}

// Launches Genetic and Genetic_multi to calculate relative error.
long double measureGeneticErrorRelativeToMulti(const unsigned numOfTests, const TSP& tsp,
        const unsigned populationSize, const long double mutationProbability,
        const unsigned numberOfGenerations)
{
    return measureAverageRelativeError_multi(numOfTests,
            [&](){return tsp.genetic_multi(populationSize, mutationProbability,
                    numberOfGenerations);},
            [&](){return tsp.genetic(populationSize, mutationProbability, numberOfGenerations);}
    );
}

}

#endif /* PROJECTUTILITIES_HPP_ */
