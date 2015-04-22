#ifndef UNDIRECTEDGRAPH_HPP_
#define UNDIRECTEDGRAPH_HPP_

#include <ostream>
#include <vector>

class UndirectedGraph
{
public:
    UndirectedGraph() = delete;

    // Creates a graph with numOfVertices vertices and no edges
    UndirectedGraph(const unsigned numOfVertices);

    // Creates a full graph with numOfVertices vertices
    // and random edges with cost <minCost; maxCost>
    UndirectedGraph(const unsigned numOfVertices, const unsigned minCost, const unsigned maxCost);

    UndirectedGraph(std::string filePath);
    UndirectedGraph(const UndirectedGraph& rhs) = default;
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

    friend std::ostream& operator<<(std::ostream& os, const UndirectedGraph& rhs);

private:
    void swap(UndirectedGraph& rhs);

    unsigned numOfVertices_ = 0U;
    unsigned numOfEdges_ = 0U;
    unsigned sumOfWeights_ = 0U;
    using Matrix = std::vector<std::vector<unsigned>>;
    Matrix matrix_;
};

#endif /* UNDIRECTEDGRAPH_HPP_ */
