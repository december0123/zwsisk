#include "TSP.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

using testing::ElementsAre;

class TravellingSalesmanProblemFixture : public ::testing::Test
{
protected:
    void SetUp()
    {
        const std::string filePath{"/home/dec/studia/sem6/zwsisk/graph_full_matrix.txt"};
        tsp_ = std::make_unique<TSP>(filePath);
    }

    std::unique_ptr<TSP> tsp_;
};

TEST_F(TravellingSalesmanProblemFixture, canBeCreatedFromFile)
{
    ASSERT_EQ(16, tsp_->getSumOfCosts());
    ASSERT_EQ(4, tsp_->getNumOfCities());
    ASSERT_EQ(4, tsp_->getCostBetweenCities(1, 2));
}

TEST_F(TravellingSalesmanProblemFixture, findsOptimalPath_bruteForce)
{
    Solution s = tsp_->bruteForce();
    ASSERT_EQ(4, s.cost_);
    ASSERT_THAT(s.route_, ElementsAre(0, 1, 3, 2));
}

TEST_F(TravellingSalesmanProblemFixture, findsAPath_genetic)
{
    Solution s = tsp_->genetic(50, 0.5, 100);
    ASSERT_TRUE(s.cost_);
    Route test(s.route_.size());
    std::iota(test.begin(), test.end(), 0);
    std::sort(s.route_.begin(), s.route_.end());
    ASSERT_TRUE(test == s.route_);
}
