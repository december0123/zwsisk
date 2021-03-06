#ifndef TSP_HPP_
#define TSP_HPP_

#include "UndirectedGraph.hpp"

#include <mutex>
#include <random>
#include <string>
#include <utility>
#include <vector>

using Graph = UndirectedGraph;
using Route = std::vector<unsigned>;
using Parents = std::pair<Route, Route>;
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
    Solution genetic(const unsigned populationSize, const long double mutationProbability,
            const unsigned numOfGenerations, Population pop = Population(0)) const;

    Solution genetic_multi(const unsigned populationSize, const long double mutationProbability,
            const unsigned numOfGenerations) const;

    void printGraph() const;
    Route createOffspring(Route parent_a, Route parent_b) const;

private:
    const Graph graph_;
    const unsigned numOfCities_ = 0U;
    const unsigned sumOfCosts_ = 0U;
    mutable std::mt19937_64 randomGen_{std::random_device{}()};
    mutable std::mutex m_;
    unsigned calcCostOfRoute(const Route& route) const;

    Population generateInitPopulation(const unsigned populationSize) const;
    Parents pickParents(const Population& population) const;

    void mutate(Route& route) const;
    bool routeContainsCity(const Route& route, const unsigned city) const;
    Route getFittest(const Population& population) const;
};

#endif /* TSP_HPP_ */
