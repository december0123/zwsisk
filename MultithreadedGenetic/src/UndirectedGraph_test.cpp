#include "UndirectedGraph.hpp"

#include <memory>
#include <regex>
#include <sstream>
#include <utility>

#include <gtest/gtest.h>

class UndirectedGraphFixture: public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        g_ = UndirectedGraph(5);
    }

    UndirectedGraph g_{5};
};

TEST_F(UndirectedGraphFixture, returnsNumberOfVertices)
{
    ASSERT_EQ(5, g_.getNumberOfVertices());
}

TEST_F(UndirectedGraphFixture, checksIfEdgeExists)
{
    ASSERT_FALSE(g_.edgeExists(0, 1));
    ASSERT_FALSE(g_.edgeExists(1, 0));
    g_.addEdge(0, 1, 7);
    ASSERT_TRUE(g_.edgeExists(0, 1));
    ASSERT_TRUE(g_.edgeExists(1, 0));
    ASSERT_FALSE(g_.edgeExists(0, 5));
    ASSERT_FALSE(g_.edgeExists(1321, 250));
}

TEST_F(UndirectedGraphFixture, addsEdge)
{
    g_.addEdge(0, 1, 7);
    ASSERT_TRUE(g_.edgeExists(0, 1));
    ASSERT_TRUE(g_.edgeExists(1, 0));
}

TEST_F(UndirectedGraphFixture, throwsOnAddingExistentEdges)
{
    g_.addEdge(0, 1, 7);
    ASSERT_THROW(g_.addEdge(0, 1, 13), std::runtime_error);
    ASSERT_THROW(g_.addEdge(1, 0, 13), std::runtime_error);
    ASSERT_NO_THROW(g_.addEdge(4, 1, 13));
}

TEST_F(UndirectedGraphFixture, removesEdge)
{
    g_.addEdge(0, 1, 7);
    ASSERT_TRUE(g_.edgeExists(0, 1));
    ASSERT_TRUE(g_.edgeExists(1, 0));

    g_.removeEdge(0, 1);
    ASSERT_FALSE(g_.edgeExists(0, 1));
    ASSERT_FALSE(g_.edgeExists(1, 0));
}

TEST_F(UndirectedGraphFixture, throwsOnRemovingNonexistentEdges)
{
    ASSERT_THROW(g_.removeEdge(0, 1), std::runtime_error);
    g_.addEdge(0, 1, 7);
    ASSERT_NO_THROW(g_.removeEdge(0, 1));
}

TEST_F(UndirectedGraphFixture, returnsNumberOfEdges)
{
    ASSERT_EQ(0, g_.getNumberOfEdges());
    g_.addEdge(0, 1, 7);
    ASSERT_EQ(1, g_.getNumberOfEdges());
    g_.removeEdge(0, 1);
    ASSERT_EQ(0, g_.getNumberOfEdges());
}

TEST_F(UndirectedGraphFixture, returnsSumOfWeights)
{
    ASSERT_EQ(0, g_.getSumOfWeights());
    g_.addEdge(0, 1, 7);
    g_.addEdge(0, 4, 12);
    g_.addEdge(4, 3, 3);
    ASSERT_EQ(22, g_.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, returnsWeightOfEdge)
{
    ASSERT_THROW(g_.getWeightOfEdge(0, 1), std::runtime_error);
    g_.addEdge(0, 1, 7);
    ASSERT_EQ(7, g_.getWeightOfEdge(0, 1));
}

TEST_F(UndirectedGraphFixture, canBeCopied)
{
    g_.addEdge(0, 1, 7);
    g_.addEdge(3, 2, 123);
    UndirectedGraph copy { g_ };
    ASSERT_EQ(g_.getWeightOfEdge(0, 1), copy.getWeightOfEdge(0, 1));
    ASSERT_EQ(g_.getWeightOfEdge(2, 3), copy.getWeightOfEdge(2, 3));
    ASSERT_EQ(g_.getNumberOfEdges(), copy.getNumberOfEdges());
    ASSERT_EQ(g_.getNumberOfVertices(), copy.getNumberOfVertices());
    ASSERT_EQ(g_.getSumOfWeights(), copy.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeCopyAssigned)
{
    g_.addEdge(0, 1, 7);
    g_.addEdge(3, 2, 123);
    UndirectedGraph copy(3);
    copy = g_;
    ASSERT_EQ(g_.getWeightOfEdge(0, 1), copy.getWeightOfEdge(0, 1));
    ASSERT_EQ(g_.getWeightOfEdge(2, 3), copy.getWeightOfEdge(2, 3));
    ASSERT_EQ(g_.getNumberOfEdges(), copy.getNumberOfEdges());
    ASSERT_EQ(g_.getNumberOfVertices(), copy.getNumberOfVertices());
    ASSERT_EQ(g_.getSumOfWeights(), copy.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeMoved)
{
    g_.addEdge(0, 1, 7);
    g_.addEdge(3, 2, 123);
    UndirectedGraph moved { std::move(g_) };
    ASSERT_EQ(7, moved.getWeightOfEdge(0, 1));
    ASSERT_EQ(123, moved.getWeightOfEdge(2, 3));
    ASSERT_EQ(2, moved.getNumberOfEdges());
    ASSERT_EQ(5, moved.getNumberOfVertices());
    ASSERT_EQ(130, moved.getSumOfWeights());

    ASSERT_FALSE(g_.edgeExists(0, 1));
    ASSERT_FALSE(g_.edgeExists(1, 0));
    ASSERT_FALSE(g_.edgeExists(2, 3));
    ASSERT_FALSE(g_.edgeExists(3, 2));
    ASSERT_EQ(0, g_.getNumberOfEdges());
    ASSERT_EQ(0, g_.getNumberOfVertices());
    ASSERT_EQ(0, g_.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeMoveAssigned)
{
    g_.addEdge(0, 1, 7);
    g_.addEdge(3, 2, 123);
    UndirectedGraph moved(3);
    moved = std::move(g_);
    ASSERT_EQ(7, moved.getWeightOfEdge(0, 1));
    ASSERT_EQ(123, moved.getWeightOfEdge(2, 3));
    ASSERT_EQ(2, moved.getNumberOfEdges());
    ASSERT_EQ(5, moved.getNumberOfVertices());
    ASSERT_EQ(130, moved.getSumOfWeights());

    ASSERT_FALSE(g_.edgeExists(0, 1));
    ASSERT_FALSE(g_.edgeExists(1, 0));
    ASSERT_FALSE(g_.edgeExists(2, 3));
    ASSERT_FALSE(g_.edgeExists(3, 2));
    ASSERT_EQ(0, g_.getNumberOfEdges());
    ASSERT_EQ(0, g_.getNumberOfVertices());
    ASSERT_EQ(0, g_.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeConstructedFromFile_fullMatrix)
{
    const std::string filePath{"/home/dec/studia/sem6/zwsisk/graph_full_matrix.txt"};
    g_ = UndirectedGraph(filePath);
    std::stringstream ss;
    ss << g_;
    const std::string graphPrint { ss.str() };
    ss.str("");
    ss.clear();
    ss << "         0         1         1         8\n";
    ss << "         1         0         4         1\n";
    ss << "         1         4         0         1\n";
    ss << "         8         1         1         0\n";
    ASSERT_EQ(ss.str(), graphPrint);
    ASSERT_EQ(6, g_.getNumberOfEdges());
    ASSERT_EQ(4, g_.getNumberOfVertices());
    ASSERT_EQ(16, g_.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeConstructedFromFile_lower_triangular)
{
    const std::string filePath{"/home/dec/studia/sem6/zwsisk/graph_lower.txt"};
    g_ = UndirectedGraph(filePath);
    std::stringstream ss;
    ss << g_;
    const std::string graphPrint { ss.str() };
    ss.str("");
    ss.clear();
    ss << "         0         1         1         8\n";
    ss << "         1         0         4         1\n";
    ss << "         1         4         0         1\n";
    ss << "         8         1         1         0\n";
    ASSERT_EQ(ss.str(), graphPrint);
    ASSERT_EQ(6, g_.getNumberOfEdges());
    ASSERT_EQ(4, g_.getNumberOfVertices());
    ASSERT_EQ(16, g_.getSumOfWeights());
}

TEST_F(UndirectedGraphFixture, canBeConstructedAsARandomInstanceAndPrintsNicely)
{
    constexpr unsigned NUM_OF_VERTICES = 3;
    constexpr unsigned MIN_COST = 2;
    constexpr unsigned MAX_COST = 7;
    g_ = UndirectedGraph(NUM_OF_VERTICES, MIN_COST, MAX_COST);
    std::stringstream ss;
    ss << g_;
    const std::string graphPrint{ss.str()};
    ss.str("");
    ss.clear();
    const std::string vertex = "         \[" + std::to_string(MIN_COST) +
            "-" + std::to_string(MAX_COST) +"]";
    const std::string zero = "         0";
    ss << zero + vertex + vertex + "\n";
    ss << vertex + zero + vertex + "\n";
    ss << vertex + vertex + zero + "\n";
    std::regex pattern{ss.str()};
    ASSERT_TRUE(std::regex_match (graphPrint, pattern));
    ASSERT_TRUE(g_.getSumOfWeights() >= NUM_OF_VERTICES * MIN_COST);
}

TEST_F(UndirectedGraphFixture, throwsOnNonexistentFile)
{
    ASSERT_THROW(std::make_unique<UndirectedGraph>("/home/dec/studia/sem6/zwsisk/BRAKUJACY.txt"),
            std::runtime_error);
}

TEST_F(UndirectedGraphFixture, throwsOnMalformedFile)
{
    ASSERT_THROW(std::make_unique<UndirectedGraph>("/home/dec/studia/sem6/zwsisk/ZLY.txt"),
            std::runtime_error);
}

TEST_F(UndirectedGraphFixture, clears)
{
    g_.clear();
    ASSERT_EQ(0, g_.getNumberOfEdges());
    ASSERT_EQ(0, g_.getSumOfWeights());
    ASSERT_THROW(g_.getWeightOfEdge(0, 1),
            std::runtime_error);
}
