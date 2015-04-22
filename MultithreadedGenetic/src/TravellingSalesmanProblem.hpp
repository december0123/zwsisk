#ifndef TRAVELLINGSALESMANPROBLEM_HPP_
#define TRAVELLINGSALESMANPROBLEM_HPP_

#include "UndirectedGraph.hpp"

#include <string>
#include <vector>

using Graph = UndirectedGraph;
using Route = std::vector<unsigned>;

struct Solution
{
    unsigned cost_ = 0U;
    Route route_;
};

class TravellingSalesmanProblem
{
public:
    TravellingSalesmanProblem() = delete;
    TravellingSalesmanProblem(const unsigned numOfCities);
    TravellingSalesmanProblem(const unsigned numOfCities, const unsigned minCost,
            const unsigned maxCost);
    TravellingSalesmanProblem(std::string pathToFile);
    ~TravellingSalesmanProblem() = default;

    unsigned getSumOfCosts() const;
    unsigned getNumOfCities() const;
    unsigned getCostBetweenCities(const unsigned from, const unsigned to) const;

    Solution bruteForce() const;
    Solution genetic(const unsigned initPopulationSize, const unsigned mutationRate,
            const unsigned numOfGenerations) const;
private:
    Graph graph_;
    unsigned numOfCities_ = 0U;
    unsigned sumOfCosts_ = 0U;

    unsigned calcCostOfRoute(const Route& route) const;

    Route generatePrimitiveRoute() const;
};

#endif /* TRAVELLINGSALESMANPROBLEM_HPP_ */
