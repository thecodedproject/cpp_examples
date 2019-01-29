// Creating this example of macking as a practice before programming exericise...

#include <gmock/gmock.h>

#include <memory>

class IOperation
{
public:
    virtual ~IOperation() = default;
    virtual int exec(int) const = 0;
};

class Multiplier : public IOperation
{
public:
    Multiplier(int factor) :
        factor_(std::move(factor))
    {
    }

    virtual int exec(int n) const override
    {
        return n*factor_;
    }

private:
    int factor_;
};

int perform(
    IOperation const& op,
    int input)
{
    return op.exec(input);
}

class MockOperation : public IOperation
{
public:
    MOCK_CONST_METHOD1(exec, int(int));
};

using namespace ::testing;

class TestMockingExample : public ::testing::Test
{
};

TEST_F(TestMockingExample, useMultiplierWithFactorOf2)
{
    auto m = Multiplier(2);
    EXPECT_EQ(10, m.exec(5));
}

TEST_F(TestMockingExample, useMultiplierWithFactorOf3)
{
    auto m = Multiplier(3);
    EXPECT_EQ(30, m.exec(10));
}

TEST_F(TestMockingExample, performWithOperation)
{
    auto mock_op = std::unique_ptr<MockOperation>(new MockOperation);

    int op_input{5};
    int op_return_value{21};

    EXPECT_CALL(*mock_op, exec(op_input))
        .WillOnce(Return(op_return_value));

    auto r = perform(*mock_op, 5);

    EXPECT_EQ(
        op_return_value,
        r
    );
}

TEST_F(TestMockingExample, performMultipleTimesWithSameOperation)
{
    auto mock_op = std::unique_ptr<MockOperation>(new MockOperation);

    int op_input{5};
    int op_return_value{21};

    EXPECT_CALL(*mock_op, exec(op_input))
        .WillRepeatedly(Return(op_return_value));


    for(auto i : {0,1,2,3,4,5})
    {
        EXPECT_EQ(
            op_return_value,
            perform(*mock_op, 5)
        );
    }
}

TEST_F(TestMockingExample, performMultipleTimesWithDifferentValuesForSameOperation)
{
    auto mock_op = std::unique_ptr<MockOperation>(new MockOperation);

    {
        InSequence s;
        EXPECT_CALL(*mock_op, exec(1))
            .WillOnce(Return(11));
        EXPECT_CALL(*mock_op, exec(2))
            .WillOnce(Return(22));
        EXPECT_CALL(*mock_op, exec(3))
            .WillOnce(Return(33));
        EXPECT_CALL(*mock_op, exec(4))
            .WillOnce(Return(44));
        EXPECT_CALL(*mock_op, exec(5))
            .WillOnce(Return(55));
    }

    EXPECT_EQ(11, perform(*mock_op, 1));
    EXPECT_EQ(22, perform(*mock_op, 2));
    EXPECT_EQ(33, perform(*mock_op, 3));
    EXPECT_EQ(44, perform(*mock_op, 4));
    EXPECT_EQ(55, perform(*mock_op, 5));
}
