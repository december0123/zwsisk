#ifndef PROJECTUTILITIES_HPP_
#define PROJECTUTILITIES_HPP_

#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <thread>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Micro = std::micro;
using Milli = std::milli;
using Seconds = std::ratio<1>;

namespace ProjectUtilities
{

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

long double measureAverageGeneticTime(const unsigned numOfTests, const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
{
    const TravellingSalesmanProblem tsp(numOfCities, minCost, maxCost);
    return measureAverageTime<Milli, Solution>(numOfTests,
            std::bind(&TravellingSalesmanProblem::genetic, &tsp, 50, 0.5, 100));
}

long double measureAverageBruteForceTime(const unsigned numOfTests, const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
{
    const TravellingSalesmanProblem tsp(numOfCities, minCost, maxCost);
    return measureAverageTime<Milli, Solution>(numOfTests,
            std::bind(&TravellingSalesmanProblem::bruteForce, &tsp));
}

template<typename T>
long double measureAverageRelativeError(const int numOfTests, const std::function<T()>& f,
        const std::function<T()>& f2)
{
    const unsigned numOfThreads = std::thread::hardware_concurrency();

    std::atomic<unsigned> sumOfGenCosts { 0 };
    auto mGen = [&]()
    {
        for (unsigned i = 0; i < numOfTests / numOfThreads; ++i)
        {
            sumOfGenCosts += f2().cost_;
        }
    };

    std::vector<std::thread> threads(numOfThreads);
    for (auto& t : threads)
    {
        t = std::thread { mGen };
    }

    const long double bf = f().cost_;

    for (auto& t : threads)
    {
        t.join();
    }

    long double genCost { static_cast<long double>(sumOfGenCosts.load()) / numOfTests };

    return std::abs((bf - genCost) / genCost);
}

/*
 * Launches BruteForce and Genetic to calculate relative error.
 * Remember that BruteForce is O(n!) so use it carefuly.
 */
long double measureGeneticQuality(const unsigned numOfTests, const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
{
    const TravellingSalesmanProblem tsp(numOfCities, minCost, maxCost);
    return measureAverageRelativeError<Solution>(numOfTests,
            std::bind(&TravellingSalesmanProblem::bruteForce, &tsp),
            std::bind(&TravellingSalesmanProblem::genetic, &tsp, 200, 0.5, 100));
}

}

#endif /* PROJECTUTILITIES_HPP_ */
