#include <gmock/gmock.h>

#include <algorithm>
#include <list>

class TestAlgorithmTransform : public ::testing::Test
{
};

TEST_F(TestAlgorithmTransform, transformListWhenOutputListIsAlreadyAllocated)
{
    auto input = std::list<int>{1, 2, 3, 4, 5};

    auto output = input;

    std::transform(
        std::begin(input),
        std::end(input),
        std::begin(output),
        [](auto const& i){
            return i + 5;
        });

    auto expected = std::list<int>{6, 7, 8, 9, 10};

    EXPECT_EQ(expected, output);
}

TEST_F(TestAlgorithmTransform, transformListInPlace)
{
    auto input = std::list<int>{1, 2, 3, 4, 5, 6};

    std::transform(
        std::begin(input),
        std::end(input),
        std::begin(input),
        [](auto const& i){
            return i*2;
        });

    auto expected = std::list<int>{2, 4, 6, 8, 10, 12};

    EXPECT_EQ(expected, input);
}

TEST_F(TestAlgorithmTransform, transformListAndInsertIntoNewList)
{
    auto input = std::list<int>{1, 2, 3, 4, 5};

    auto output = std::list<int>{};;

    std::transform(
        std::begin(input),
        std::end(input),
        std::back_inserter(output),
        [](auto const& i){
            return i + 5;
        });

    auto expected = std::list<int>{6, 7, 8, 9, 10};

    EXPECT_EQ(expected, output);
}
