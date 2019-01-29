#include <gmock/gmock.h>

#include <list>
#include <queue>
#include <thread>

using Message = std::function<void()>;

class MessageQueue
{
public:
    void send(Message m)
    {
        std::lock_guard lock(mut_);
        queue_.emplace(std::move(m));
    }

    Message receive()
    {
        std::lock_guard lock(mut_);
        if(!queue_.empty())
        {
            auto m = queue_.front();
            queue_.pop();
            return m;
        }
        else
        {
            return []{};
        }
    }

private:
    std::mutex mut_;
    std::queue<Message> queue_;
};

class Active
{
public:
    Active() :
        thd{[=]{
            while(!done){
                mq.receive()();
            }
        }}
    {
    }

    ~Active()
    {
        send([=]{done = true;});
        thd.join();
    }

    void send(std::function<void()> m)
    {
        mq.send(m);
    }

private:

    bool done = false;
    MessageQueue mq;
    std::thread thd;
};

using namespace ::testing;
class TestActiveConsumer : public Test
{
};

TEST_F(TestActiveConsumer, simpleUseOfActveConsumer)
{
    int a{0};
    int b{0};
    int c{0};
    int d{0};

    {
        Active consumer;

        consumer.send([&]{
            a = 10;
            std::cout << "a" << std::endl;
        });
        consumer.send([&]{
            b = a*50;
            std::cout << "b" << std::endl;
        });
        consumer.send([&]{
            c = a-b;
            std::cout << "c" << std::endl;
        });
        consumer.send([&]{
            d = c*a*b;
            std::cout << "d" << std::endl;
        });
    }

    std::cout << "a " << a << std::endl;
    std::cout << "b " << b << std::endl;
    std::cout << "c " << c << std::endl;
    std::cout << "d " << d << std::endl;
}

class Stage
{
public:
    Stage(std::function<void(std::string&)> f) :
        f_{f}
    {
    }

    void process(std::string & buffer)
    {
        a.send([&](){
            f_(buffer);
        });
    }

private:
    std::function<void(std::string&)> f_;
    Active a;
};

TEST_F(TestActiveConsumer, pipelineOfConsumerActingOnBuffer)
{
    auto buffers = std::list<std::string>(1000, std::string{});

    {
        Stage step4([](std::string & b){
            b +=" step4";
        });
        Stage step3([&](std::string & b){
            b += " step3";
            step4.process(b);
        });
        Stage step2([&](std::string & b){
            b += " step2";
            step3.process(b);
        });
        Stage step1([&](std::string & b){
            b += "step1";
            step2.process(b);
        });


        for(auto & b : buffers)
        {
            step1.process(b);
        }
    }

    for(auto const& b : buffers)
    {
        EXPECT_EQ("step1 step2 step3 step4", b);
    }
}
