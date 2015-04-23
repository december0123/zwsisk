#include "TravellingSalesmanProblem.hpp"

#include <algorithm>

TravellingSalesmanProblem::TravellingSalesmanProblem(const unsigned numOfCities)
        : graph_(numOfCities), numOfCities_(numOfCities), sumOfCosts_(graph_.getSumOfWeights()) {}

TravellingSalesmanProblem::TravellingSalesmanProblem(const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
        : graph_ { numOfCities, minCost, maxCost }, numOfCities_(numOfCities),
          sumOfCosts_(graph_.getSumOfWeights()) {}

TravellingSalesmanProblem::TravellingSalesmanProblem(std::string pathToFile)
        : graph_(pathToFile), numOfCities_(graph_.getNumberOfVertices()),
          sumOfCosts_(graph_.getSumOfWeights()) {}

unsigned TravellingSalesmanProblem::getSumOfCosts() const
{
    return sumOfCosts_;
}

unsigned TravellingSalesmanProblem::getNumOfCities() const
{
    return numOfCities_;
}

unsigned TravellingSalesmanProblem::getCostBetweenCities(const unsigned from,
        const unsigned to) const
{
    return graph_.getWeightOfEdge(from, to);
}

Solution TravellingSalesmanProblem::bruteForce() const
{
    unsigned shortestDistance_ = std::numeric_limits<unsigned>::max();
    Route bestRoute_;
    Route route { generatePrimitiveRoute() };

    unsigned currentDistance = 0U;
    do
    {
        currentDistance = calcCostOfRoute(route);
        if (currentDistance < shortestDistance_)
        {
            shortestDistance_ = currentDistance;
            bestRoute_ = route;
        }
    }
    while (std::next_permutation(route.begin(), route.end()));

    return {shortestDistance_, bestRoute_};
}

unsigned TravellingSalesmanProblem::calcCostOfRoute(const Route& route) const
{
    unsigned cost = 0U;
    for (unsigned i = 0; i < route.size() - 1; ++i)
    {
        cost += graph_.getWeightOfEdge(route[i], route[i + 1]);
    }
    cost += graph_.getWeightOfEdge(route.front(), route.back());
    return cost;
}

Solution TravellingSalesmanProblem::genetic(const unsigned initPopulationSize,
        const unsigned mutationRate, const unsigned numOfGenerations) const
{
    Route route {generatePrimitiveRoute()};
    Route bestRoute;
    return {};
}

Route TravellingSalesmanProblem::generatePrimitiveRoute() const
{
    Route route(numOfCities_);
    for (unsigned i = 0; i < numOfCities_; ++i)
    {
        route[i] = i;
    }
    return route;
}
