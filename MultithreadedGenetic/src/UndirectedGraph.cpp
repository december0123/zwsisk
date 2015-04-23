#include "UndirectedGraph.hpp"

#include <fstream>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <string>

UndirectedGraph::UndirectedGraph(const unsigned numOfVertices)
        : numOfVertices_ { numOfVertices }, matrix_{numOfVertices,
            std::vector<unsigned>(numOfVertices, 0 )} {}

UndirectedGraph::UndirectedGraph(const unsigned numOfVertices, const unsigned minCost,
        const unsigned maxCost)
        : numOfVertices_ { numOfVertices }, matrix_ { numOfVertices }
{
    std::uniform_int_distribution<unsigned> distr { minCost, maxCost };
    std::mt19937_64 randomGen(std::random_device { }());
    unsigned weight;
    for (auto& i : matrix_)
    {
        for (int j = 0; j < numOfVertices; ++j)
        {
            weight = distr(randomGen);
            sumOfWeights_ += weight;
            i.emplace_back(weight);
        }
    }
}

UndirectedGraph::UndirectedGraph(std::string filePath)
{
    std::ifstream file(filePath);
    if (file)
    {
        std::string line;
        do
        {
            std::getline(file, line);
            if (line.find("DIMENSION") == 0)
            {
                std::string value = line.substr(line.find(':') + 1);
                numOfVertices_ = std::stoi(value);
                matrix_ = {numOfVertices_, std::vector<unsigned>(numOfVertices_, 0)};
            }
        }
        while ("EDGE_WEIGHT_SECTION" != line);

        unsigned weight = 0;
        for (unsigned row = 0; row < numOfVertices_; ++row)
        {
            for (unsigned column = 0; column < numOfVertices_; ++column)
            {
                if (!(file >> weight))
                {
                    throw std::runtime_error { " * Malformed file * " };
                }
                if (weight)
                {
                    try
                    {
                        addEdge(row, column, weight);
                    }
                    catch (const std::runtime_error& e)
                    {
                        // format pliku tego wymaga
                    }
                }
            }
        }
    }
    else
    {
        throw std::runtime_error { " * Couldn't open given file * " };
    }
}

UndirectedGraph::UndirectedGraph(const UndirectedGraph& rhs)
    : numOfVertices_{rhs.numOfVertices_}, numOfEdges_{rhs.numOfEdges_},
      sumOfWeights_{rhs.sumOfWeights_}, matrix_{rhs.matrix_} {}

UndirectedGraph::UndirectedGraph(UndirectedGraph&& rhs)
    : numOfVertices_{rhs.numOfVertices_}, numOfEdges_{rhs.numOfEdges_},
      sumOfWeights_{rhs.sumOfWeights_}, matrix_{std::move(rhs.matrix_)}
{
    rhs.numOfVertices_ = 0U;
    rhs.numOfEdges_ = 0U;
    rhs.sumOfWeights_ = 0U;
}

UndirectedGraph& UndirectedGraph::operator=(UndirectedGraph rhs)
{
    swap(rhs);
    return *this;
}

void UndirectedGraph::addEdge(const unsigned from, const unsigned to, const unsigned weight)
{
    if (edgeExists(from, to))
    {
        std::string excMsg { "Edge from " + std::to_string(from) + " to " + std::to_string(to)
                + " already exists." };
        throw std::runtime_error { excMsg };
    }
    sumOfWeights_ += weight;
    matrix_[from][to] = weight;
    matrix_[to][from] = weight;
    ++numOfEdges_;
}

void UndirectedGraph::removeEdge(const unsigned from, const unsigned to)
{
    if (!edgeExists(from, to))
    {
        std::string excMsg { "Edge from " + std::to_string(from) + " to " + std::to_string(to)
                + " does not exist." };
        throw std::runtime_error { std::move(excMsg) };
    }

    sumOfWeights_ -= matrix_[from][to];
    matrix_[from][to] = 0U;
    matrix_[to][from] = 0U;
    --numOfEdges_;
}

bool UndirectedGraph::edgeExists(const unsigned from, const unsigned to) const
{
    if (from >= numOfVertices_ || to >= numOfVertices_)
    {
        return false;
    }
    return matrix_[from][to];
}

unsigned UndirectedGraph::getWeightOfEdge(const unsigned from, const unsigned to) const
{
    if (!edgeExists(from, to))
    {
        std::string excMsg { "Edge from " + std::to_string(from) + " to " + std::to_string(to)
                + " does not exist." };
        throw std::runtime_error { std::move(excMsg) };
    }
    return matrix_[from][to];
}

unsigned UndirectedGraph::getNumberOfVertices() const
{
    return numOfVertices_;
}

unsigned UndirectedGraph::getNumberOfEdges() const
{
    return numOfEdges_;
}

unsigned UndirectedGraph::getSumOfWeights() const
{
    return sumOfWeights_;
}

void UndirectedGraph::clear()
{
    numOfVertices_ = 0U;
    numOfEdges_ = 0U;
    sumOfWeights_ = 0U;
    matrix_ = {numOfVertices_, std::vector<unsigned>(numOfVertices_, 0)};
}

void UndirectedGraph::swap(UndirectedGraph& rhs)
{
    using std::swap;
    swap(numOfVertices_, rhs.numOfVertices_);
    swap(numOfEdges_, rhs.numOfEdges_);
    swap(sumOfWeights_, rhs.sumOfWeights_);
    swap(matrix_, rhs.matrix_);
}

std::ostream& operator<<(std::ostream& os, const UndirectedGraph& rhs)
{
    for (int i = 0; i < rhs.numOfVertices_; ++i)
    {
        for (int j = 0; j < rhs.numOfVertices_; ++j)
        {
            os << std::setw(10) << rhs.matrix_[i][j];
        }
        os << std::endl;
    }
    return os;
}
