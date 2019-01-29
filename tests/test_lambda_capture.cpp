#include <gmock/gmock.h>

#include <functional>

class TestLambdaCapture : public ::testing::Test
{
};

TEST_F(TestLambdaCapture, captureByRefAndExecAfterRefChanged)
{
    auto i = 10;
    auto f = [&]{ return i; };

    i = 5;
    EXPECT_EQ(5, f());
}

class CaptureThis
{
public:
    void setupCaptureWithCopy()
    {
        f = [*this]{ return i; };
    }

    void setupCaptureWithReference()
    {
        f = [this]{ return i; };
    }

    int exec()
    {
        return f();
    }

    int i = 10;
    std::function<int()> f;
};

TEST_F(TestLambdaCapture, captureThisByCopy)
{
    CaptureThis a;
    a.setupCaptureWithCopy();
    a.i = 20;
    EXPECT_EQ(10, a.exec());
}

TEST_F(TestLambdaCapture, captureThisByReference)
{
    CaptureThis a;
    a.setupCaptureWithReference();
    a.i = 20;
    EXPECT_EQ(20, a.exec());
}
