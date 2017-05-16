#ifndef MTASK_H_
#define MTASK_H_

#include <functional>
#include <future>
#include <thread>
#include <type_traits>

#define _NOEXCEPT	       noexcept
#define MAX_PRIORITY       100
#define MIN_PRIORITY       0
#define DEFAULT_PRIORITY   50

class MTask
{
public:
    ~MTask() _NOEXCEPT
    {	// clean up
    }
    
    template<class _Fn, class... _Args>
    explicit MTask(_Fn&& _Fx, _Args&&... _Ax)
    {	// construct with _Fx(_Ax...)
        auto oriFunc = _STD bind(_Decay_copy(_STD forward<_Fn>(_Fx)),
            _Decay_copy(_STD forward<_Args>(_Ax))...);

        using RetType = decltype(oriFunc());

        auto task = std::make_shared<std::packaged_task<RetType()>>(oriFunc);

        mFunc = [task]()
        {
            (*task)();
        };
        
        mPriority = DEFAULT_PRIORITY;
    }

    void execute()
    {
        mFunc();
    }

    void setPriority(int priority)
    {
        mPriority = priority;
        if (priority < MIN_PRIORITY)
            mPriority = MIN_PRIORITY;
        if (priority > MAX_PRIORITY)
            mPriority = MAX_PRIORITY;
    }

    bool operator<(const MTask &t)const
    {
        return mPriority < t.mPriority;
    }

private:
    MTask() _NOEXCEPT
    {	//  
        mPriority = DEFAULT_PRIORITY;
    }

private:
    int mPriority;
    std::function<void()> mFunc;
};
#endif //end of MTASK_H_
