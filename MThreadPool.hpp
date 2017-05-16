#ifndef MTHREADPOOL_H_
#define MTHREADPOOL_H_

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

class MTask;
bool operator<(const std::shared_ptr<MTask> &a, const std::shared_ptr<MTask> &b);

class MThreadPool
{
public:
    static MThreadPool* sharedInstance();
    ~MThreadPool();
    
    void addTask(const std::shared_ptr<MTask> t);

    template<class _Fn, class... _Args>
    void addTask(_Fn&& _Fx, _Args&&... _Ax)
    {
        std::shared_ptr<MTask> t(new MTask(_Fx, _Ax...));
        addTask(t);
    }

    int stop();
    int taskSize()const;

private:
    MThreadPool(int threadNum = 1);
    void run();

private:
    std::vector<std::thread> threadVec;
    std::priority_queue<std::shared_ptr<MTask>> taskQueue;
    std::atomic<bool> isRunning;

    static MThreadPool* instance;
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (MThreadPool::instance)
            {
                delete MThreadPool::instance;
            }
        }
    };
    static CGarbo Garbo;
    static std::mutex mtx;
    static std::condition_variable cv;
};
#endif//end of MTHREADPOOL_H_H
