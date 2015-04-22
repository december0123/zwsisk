#include "TravellingSalesmanProblem.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

using testing::ElementsAre;

class TravellingSalesmanProblemFixture : public ::testing::Test
{
protected:
    void SetUp()
    {
        const std::string filePath{"/home/dec/studia/sem6/zwsisk/graph.txt"};
        tsp_ = std::make_unique<TravellingSalesmanProblem>(filePath);
    }

    std::unique_ptr<TravellingSalesmanProblem> tsp_;
};

TEST_F(TravellingSalesmanProblemFixture, canBeCreatedFromFile)
{
    ASSERT_EQ(6, tsp_->getSumOfCosts());
    ASSERT_EQ(3, tsp_->getNumOfCities());
    ASSERT_EQ(3, tsp_->getCostBetweenCities(1, 2));
}

TEST_F(TravellingSalesmanProblemFixture, findsOptimalPath_bruteForce)
{
    Solution s = tsp_->bruteForce();
    ASSERT_EQ(6, s.cost_);
    ASSERT_THAT(s.route_, ElementsAre(0, 1, 2));
}
