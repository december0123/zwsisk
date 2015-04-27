#include "TravellingSalesmanProblem.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <utility>

TravellingSalesmanProblem::TravellingSalesmanProblem(const unsigned numOfCities)
        : graph_(numOfCities), numOfCities_(numOfCities), sumOfCosts_(graph_.getSumOfWeights())
{}

TravellingSalesmanProblem::TravellingSalesmanProblem(const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
        : graph_ { numOfCities, minCost, maxCost }, numOfCities_(numOfCities),
          sumOfCosts_(graph_.getSumOfWeights())
{}

TravellingSalesmanProblem::TravellingSalesmanProblem(std::string pathToFile)
        : graph_(pathToFile), numOfCities_(graph_.getNumberOfVertices()),
          sumOfCosts_(graph_.getSumOfWeights())
{}

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
    unsigned shortestDistance = std::numeric_limits<unsigned>::max();
    Route bestRoute;
    Route route { generatePrimitiveRoute() };

    unsigned currentDistance = 0U;
    do
    {
        currentDistance = calcCostOfRoute(route);
        if (currentDistance < shortestDistance)
        {
            shortestDistance = currentDistance;
            bestRoute = route;
        }
    }
    while (std::next_permutation(route.begin(), route.end()));

    return {shortestDistance, bestRoute};
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

Route TravellingSalesmanProblem::generatePrimitiveRoute() const
{
    Route route(numOfCities_);
    for (unsigned i = 0; i < numOfCities_; ++i)
    {
        route[i] = i;
    }
    return route;
}

Solution TravellingSalesmanProblem::genetic(const unsigned populationSize,
        const long double mutationProbability, const unsigned numOfGenerations) const
{
    Population population { generateInitPopulation(populationSize) };
    std::uniform_real_distribution<long double> distr(0, 1);

    for (auto i = 0U; i < numOfGenerations; ++i)
    {
        Population nextGen;
        for (auto j = 0U; j < populationSize - 1; ++j)
        {
            Route parent_a = pickFitParent(population);
            Route parent_b = pickFitParent(population);
            Route offspring{createOffspring(std::move(parent_a), std::move(parent_b))};
            if (distr(randomGen_) <= mutationProbability)
            {
                mutate(offspring);
            }
            nextGen.push_back(std::move(offspring));
        }
        nextGen.shrink_to_fit();
        std::sort(nextGen.begin(), nextGen.end(),
                [this](const Route& lhs, const Route& rhs) {
                    return calcCostOfRoute(lhs) < calcCostOfRoute(rhs);
        });
        population = std::move(nextGen);
    }

    return {calcCostOfRoute(population.front()), population.front()};
}

std::vector<Route> TravellingSalesmanProblem::generateInitPopulation(
        const unsigned populationSize) const
{
    Population population;
    Route route { generatePrimitiveRoute() };

    for (unsigned i = 0; i < populationSize; ++i)
    {
        std::shuffle(route.begin(), route.end(), std::mt19937_64{std::random_device{}()});
        population.push_back(route);
    }
    population.shrink_to_fit();
    return population;
}

Route TravellingSalesmanProblem::pickFitParent(const Population& population) const
{
    const unsigned alphaSize = population.size() > 5 ? 5 : population.size();
    std::uniform_int_distribution<unsigned> distr(0, alphaSize - 1);
    Population potentialParents;
    for (auto i = 0U; i < alphaSize; ++i)
    {
        potentialParents.push_back(population[i]);
    }
    std::sort(potentialParents.begin(), potentialParents.end(),
            [this](const Route& lhs, const Route& rhs) {
                return calcCostOfRoute(lhs) < calcCostOfRoute(rhs);
    });
    return potentialParents.front();
}

Route TravellingSalesmanProblem::createOffspring(Route parent_a, Route parent_b) const
{
    std::uniform_int_distribution<unsigned> distr(0, parent_a.size() - 1);
    Route offspring(parent_a.size(), -1);
    const unsigned pivot_a = distr(randomGen_);
    unsigned pivot_b = 0U;
    do
    {
        pivot_b = distr(randomGen_);
    }
    while (pivot_b < pivot_a);

    for (auto i = pivot_a; i <= pivot_b; ++i)
    {
        offspring[i] = parent_a[i];
    }
    for (auto i = 0U; i < offspring.size(); ++i)
    {
        if ((i < pivot_a) || (i > pivot_b))
        {
            unsigned j = 0;
            while(routeContainsCity(offspring, parent_b[j]))
            {
                ++j;
            }
            offspring[i] = parent_b[j];
        }
    }
    return offspring;
}

void TravellingSalesmanProblem::mutate(Route& route) const
{
    std::uniform_int_distribution<unsigned> distr(0, route.size() - 1);
    std::swap(route[distr(randomGen_)], route[distr(randomGen_)]);
}

bool TravellingSalesmanProblem::routeContainsCity(const Route& route, const unsigned city) const
{
    return std::find(route.begin(), route.end(), city) != route.end();
}
