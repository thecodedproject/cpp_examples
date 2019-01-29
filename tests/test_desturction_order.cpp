#include <gmock/gmock.h>

class A
{
public:
    A()
    {
        std::cout << "A::A()" << std::endl;
    }

    ~A()
    {
        std::cout << "A::~A()" << std::endl;
    }
};

class B
{
public:
    B()
    {
        std::cout << "B::B()" << std::endl;
    }

    ~B()
    {
        std::cout << "B::~B()" << std::endl;
    }

    A a = {};
};

class C
{
public:
    C()
    {
        std::cout << "C::C()" << std::endl;
    }

    ~C()
    {
        std::cout << "C::~C()" << std::endl;
    }

    B b = {};
    A a = {};
};

class TestDestructionOrder : public ::testing::Test
{
};

TEST_F(TestDestructionOrder, some)
{
    C c{};
}
