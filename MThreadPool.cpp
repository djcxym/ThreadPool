#include "MThreadPool.hpp"
#include <iostream>
#include <thread>
#include "MTask.hpp"

std::mutex MThreadPool::mtx;
std::condition_variable MThreadPool::cv;

bool operator<(const std::shared_ptr<MTask> &a, const std::shared_ptr<MTask> &b)
{
    return a->operator<(*b);
}

MThreadPool::MThreadPool(int threadNum)
{
    if (threadNum <= 0)
        return;
    isRunning.store(true);
    for (int i = 0; i<threadNum; i++)
    {
        threadVec.push_back(std::thread(&MThreadPool::run, this));
    }
}


MThreadPool::~MThreadPool()
{
}

MThreadPool* MThreadPool::instance = NULL;
MThreadPool* MThreadPool::sharedInstance()
{
    if (instance == NULL)
    {
        std::unique_lock<std::mutex> lck(mtx);
        if (instance == NULL)
        {
            instance = new MThreadPool();
        }
    }
    return instance;
}

void MThreadPool::run()
{
    while (1)
    {
        std::unique_lock<std::mutex> lck(mtx);
        while (isRunning && taskQueue.empty())
        {
            std::cout << "Empty waiting..." << std::endl;
            cv.wait(lck);
        }

        if (!isRunning)
        {
            return;
        }

        if (!taskQueue.empty())
        {
            std::shared_ptr<MTask> t = taskQueue.top();
            taskQueue.pop();
            t->execute();
        }
    }
}

int MThreadPool::stop()
{
    if (!isRunning)
        return -1;
    isRunning.store(false);
    cv.notify_all();
    for (auto &th : threadVec)
        th.join();
    return 0;
}

void MThreadPool::addTask(const std::shared_ptr<MTask> t)
{
    std::unique_lock<std::mutex> lck(mtx);
    taskQueue.push(t);
    std::cout << "NOTIFY" << std::endl;
    cv.notify_one();
}

int MThreadPool::taskSize()const
{
    return taskQueue.size();
}