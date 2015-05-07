#include "TSP.hpp"

#include <algorithm>
#include <random>
#include <utility>

TSP::TSP(const unsigned numOfCities)
        : graph_(numOfCities), numOfCities_(numOfCities), sumOfCosts_(graph_.getSumOfWeights())
{}

TSP::TSP(const unsigned numOfCities,
        const unsigned minCost, const unsigned maxCost)
        : graph_ { numOfCities, minCost, maxCost }, numOfCities_(numOfCities),
          sumOfCosts_(graph_.getSumOfWeights())
{}

TSP::TSP(std::string pathToFile)
        : graph_(pathToFile), numOfCities_(graph_.getNumberOfVertices()),
          sumOfCosts_(graph_.getSumOfWeights())
{}

unsigned TSP::getSumOfCosts() const
{
    return sumOfCosts_;
}

unsigned TSP::getNumOfCities() const
{
    return numOfCities_;
}

unsigned TSP::getCostBetweenCities(const unsigned from,
        const unsigned to) const
{
    return graph_.getWeightOfEdge(from, to);
}

Solution TSP::bruteForce() const
{
    unsigned shortestDistance = std::numeric_limits<unsigned>::max();
    Route bestRoute;
    Route route(numOfCities_);
    std::iota(route.begin(), route.end(), 0);

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

unsigned TSP::calcCostOfRoute(const Route& route) const
{
    unsigned cost = 0U;
    for (unsigned i = 0; i < route.size() - 1; ++i)
    {
        cost += graph_.getWeightOfEdge(route[i], route[i + 1]);
    }
    cost += graph_.getWeightOfEdge(route.front(), route.back());
    return cost;
}

Solution TSP::genetic(const unsigned populationSize,
        const long double mutationProbability, const unsigned numOfGenerations) const
{
    Population population { std::move(generateInitPopulation(populationSize)) };
    std::uniform_real_distribution<long double> distr(0, 1);

    for (auto i = 0U; i < numOfGenerations; ++i)
    {
        Population nextGen(populationSize);
        nextGen[0] = getFittest(population);
        for (auto j = 1U; j < populationSize; ++j)
        {
            Parents p = pickParents(population);
            Route offspring{createOffspring(std::move(p.first), std::move(p.second))};
            if (distr(randomGen_) <= mutationProbability)
            {
                mutate(offspring);
            }
            nextGen[j] = std::move(offspring);
        }
        population = std::move(nextGen);
    }
    Route best{getFittest(population)};
    return {calcCostOfRoute(best), std::move(best)};
}

Population TSP::generateInitPopulation(
        const unsigned populationSize) const
{
    Population population(populationSize);
    Route route(numOfCities_);
    std::iota(route.begin(), route.end(), 0);

    for (unsigned i = 0; i < populationSize; ++i)
    {
        std::shuffle(route.begin(), route.end(), randomGen_);
        population[i] = route;
    }
    return population;
}

Parents TSP::pickParents(Population& population) const
{
    const unsigned alphaSize = population.size() > 5 ? 5 : population.size();
    std::uniform_int_distribution<unsigned> distr(0, alphaSize - 1);
    auto cmp = [this](const Route& lhs, const Route& rhs) {
        return calcCostOfRoute(lhs) < calcCostOfRoute(rhs);
    };
    std::partial_sort(population.begin(), population.begin() + alphaSize, population.end(), cmp);
    unsigned parent_a = distr(randomGen_);
    unsigned parent_b = distr(randomGen_);
    while(parent_a == parent_b)
    {
        parent_b = distr(randomGen_);
    }

    return std::make_pair(population[parent_a], population[parent_b]);
}

Route TSP::createOffspring(Route parent_a, Route parent_b) const
{
    std::uniform_int_distribution<unsigned> distr(0, parent_a.size() - 1);
    Route offspring(parent_a.size(), -1);

    const unsigned pivot_a = distr(randomGen_);
    unsigned pivot_b = distr(randomGen_);
    while (pivot_b < pivot_a)
    {
        pivot_b = distr(randomGen_);
    }

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

void TSP::mutate(Route& route) const
{
    std::uniform_int_distribution<unsigned> distr(0, route.size() - 1);
    std::swap(route[distr(randomGen_)], route[distr(randomGen_)]);
}

bool TSP::routeContainsCity(const Route& route, const unsigned city) const
{
    return std::find(route.begin(), route.end(), city) != route.end();
}

Route TSP::getFittest(const Population& population) const
{
    return *std::max_element(population.begin(), population.end(),
            [this](const Route& lhs, const Route& rhs) {
                return calcCostOfRoute(lhs) > calcCostOfRoute(rhs);
    });
}

void TSP::printGraph() const
{
    std::cout << graph_ << std::endl;
}
