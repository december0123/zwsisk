#ifndef TSP_HPP_
#define TSP_HPP_

#include "UndirectedGraph.hpp"

#include <iostream>
#include <random>
#include <string>
#include <vector>

using Graph = UndirectedGraph;
using Route = std::vector<unsigned>;
using Population = std::vector<Route>;

struct Solution
{
    unsigned cost_ = 0U;
    Route route_;
};

class TSP
{
public:
    TSP() = delete;
    TSP(const unsigned numOfCities);
    TSP(const unsigned numOfCities, const unsigned minCost,
            const unsigned maxCost);
    TSP(std::string pathToFile);
    TSP(TSP&&) = default;
    ~TSP() = default;

    unsigned getSumOfCosts() const;
    unsigned getNumOfCities() const;
    unsigned getCostBetweenCities(const unsigned from, const unsigned to) const;

    Solution bruteForce() const;
    Solution genetic(const unsigned populationSize, const long double mutationRate,
            const unsigned numOfGenerations) const;

    void printGraph() const;

private:
    Graph graph_;
    unsigned numOfCities_ = 0U;
    unsigned sumOfCosts_ = 0U;
    mutable std::mt19937_64 randomGen_{std::random_device{}()};

    unsigned calcCostOfRoute(const Route& route) const;

    Route generatePrimitiveRoute() const;
    Population generateInitPopulation(const unsigned populationSize) const;
    Route pickParent(const Population& population) const;
    Route createOffspring(Route parent_a, Route parent_b) const;
    void mutate(Route& route) const;
    bool routeContainsCity(const Route& route, const unsigned city) const;
    Route getFittest(const Population& population) const;
};

#endif /* TSP_HPP_ */
