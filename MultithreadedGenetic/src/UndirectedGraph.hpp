#ifndef UNDIRECTEDGRAPH_HPP_
#define UNDIRECTEDGRAPH_HPP_

#include <iomanip>
#include <vector>

class UndirectedGraph
{
public:
    UndirectedGraph() = default;

    // Creates a graph with numOfVertices vertices and no edges
    UndirectedGraph(const unsigned numOfVertices);

    // Creates a complete graph with numOfVertices vertices
    // and random edges with cost <minCost; maxCost>
    UndirectedGraph(const unsigned numOfVertices, const unsigned minCost, const unsigned maxCost);

    UndirectedGraph(std::string filePath);
    UndirectedGraph(const UndirectedGraph& rhs);
    UndirectedGraph(UndirectedGraph&& rhs);
    ~UndirectedGraph() = default;

    UndirectedGraph& operator=(UndirectedGraph rhs);

    void addEdge(const unsigned from, const unsigned to, const unsigned weight);
    void removeEdge(const unsigned from, const unsigned to);
    bool edgeExists(const unsigned from, const unsigned to) const;
    unsigned getWeightOfEdge(const unsigned from, const unsigned to) const;
    unsigned getNumberOfVertices() const;
    unsigned getNumberOfEdges() const;
    unsigned getSumOfWeights() const;

    // Sets graph to default state as if UndirectedGraph(numOfVertices) was called
    void clear();

    template<typename T>
    friend T& operator<<(T& os, const UndirectedGraph& rhs)
    {
        for (auto i = 0U; i < rhs.numOfVertices_; ++i)
        {
            for (auto j = 0U; j < rhs.numOfVertices_; ++j)
            {
                os << std::setw(10) << rhs.matrix_[i][j];
            }
            os << std::endl;
        }
        return os;
    }

private:
    void swap(UndirectedGraph& rhs);
    std::string goToLineContaining(std::string phrase, std::ifstream& file);

    unsigned numOfVertices_ = 0U;
    unsigned numOfEdges_ = 0U;
    unsigned sumOfWeights_ = 0U;
    using Matrix = std::vector<std::vector<unsigned>>;
    Matrix matrix_;
};

#endif /* UNDIRECTEDGRAPH_HPP_ */
