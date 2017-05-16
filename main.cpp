#include <iostream>
#include "MTask.hpp"
#include "MThreadPool.hpp"
#include <chrono>

using namespace std;

int print(int a,int b)
{
    //cout << a + b << endl;
    return a + b;
}

void print2(int a, int b)
{
    cout << a + b << endl;
}

void print3(int a, int b, int c)
{
    cout << a + b + c<< endl;
}

class Base
{
public:
    int show(int a, int b)
    {
        //cout << "Base::show - " <<a + b<< endl;
        return a + b;
    }
private:
};

int main(void)
{
    std::shared_ptr<MTask> t1(new MTask(print, 1, 4));
    t1->setPriority(85);
    std::shared_ptr<MTask> t2(new MTask(print, 2, 4));
    t2->setPriority(90);
    std::shared_ptr<MTask> t3(new MTask(print, 3, 4));
    t3->setPriority(95);
    std::shared_ptr<MTask> t4(new MTask(print, 4, 4));
    t4->setPriority(100);

    MThreadPool *pool = MThreadPool::sharedInstance();

    pool->addTask(t1);
    pool->addTask(t2);
    pool->addTask(t3);
    pool->addTask(t4);

    Base bb;

    pool->addTask(print, 5, 4);

    std::shared_ptr<MTask> t5(new MTask(print2, 14, 4));
    std::shared_ptr<MTask> t6(new MTask(&Base::show, bb, 14, 4));
    std::shared_ptr<MTask> t7(new MTask(print3, 14, 4,1));
    pool->addTask(t5);
    pool->addTask(t6);
    pool->addTask(t7);

    this_thread::sleep_for(chrono::milliseconds(1000));
    while (1)
    {
        if (pool->taskSize() == 0)
        {

            cout << "TASKSIZE = 0, CALL STOP" << endl;
            if (pool->stop() == -1)
            {
                printf("Now I will exit from main/n");
                exit(0);
            }
        }
        if (pool->stop() == -1)
        {
            printf("Now I will exit from main/n");
            exit(0);
        }
    }
    cout << "END" << endl;

    return 0;
}