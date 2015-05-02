/*
 * ProjectUtilities.hpp
 *
 *  Created on: 2 maj 2015
 *      Author: dec
 */

#ifndef PROJECTUTILITIES_HPP_
#define PROJECTUTILITIES_HPP_

#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <functional>
#include <future>

using Clock = std::chrono::high_resolution_clock;
using Micro = std::micro;
using Milli = std::milli;
using Seconds = std::ratio<1>;

namespace ProjectUtilities
{

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

template<typename T>
long double measureQuality(const int numOfTests, const std::function<T()>& f,
        const std::function<T()>& f2)
{
    constexpr unsigned numOfThreads = 5;

    std::atomic<unsigned> genCost { 0 };
    auto mGen = [&]()
    {
        for (auto i = 0; i < numOfTests / numOfThreads; ++i)
        {
            genCost += f2().cost_;
        }
    };

    std::array<std::thread, numOfThreads> threads;
    ;
    for (auto& t : threads)
    {
        t = std::thread { mGen };
    }

    long double bf = f().cost_;

    for (auto& i : threads)
    {
        i.join();
    }

    long double witam { static_cast<long double>(genCost.load()) / numOfTests };

    return std::abs((bf - witam) / witam);
}

}

#endif /* PROJECTUTILITIES_HPP_ */
