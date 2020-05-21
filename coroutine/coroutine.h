//
// Created by waxz on 2020/5/21.
//

#ifndef COROUTINE_COROUTINE_H
#define COROUTINE_COROUTINE_H

#include <boost/coroutine2/all.hpp>
#include <boost/context/continuation.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <deque>

#include "clock_time.h"
#include "singleton.h"

#include "system_signal.h"

#define __co_sleep_for(sleep_time) do { \
    [&](int64_t t, int64_t next_time = 0l, int64_t current_time = 0l) {                               \
    current_time = common::ToUniversal(common::FromUnixNow());next_time = (next_time < current_time) ? current_time + 10000*t : next_time;\
        if(next_time > current_time) {\
        co_yied(next_time);}\
    }((sleep_time));}\
while(0)

#define __co_create(...)do {  \
coroutine::co_out<int64_t> co{ __VA_ARGS__ };\
    auto m_global_schedual_sinleton_instance = coroutine::SchedulerSingleton<coroutine::Scheduler>::getInstance();\
    m_global_schedual_sinleton_instance->addTask(std::move(co)); }\
while(0)


namespace coroutine{
    template <typename T>
    class SchedulerSingleton: public common::Singleton<T>{

    public:
         ~SchedulerSingleton(void){

            if(common::Singleton<T>::m_pInstance) common::Singleton<T>::DestroyInstance();
        }

    };

    template <typename T>
    using coro_t = boost::coroutines2::coroutine<T>;
// define pipe transfer int
// two end, pull, push

    template <typename T>
    using co_in = typename coro_t<T>::push_type ;

    template <typename T>
    using co_out = typename coro_t<T>::pull_type ;

    class Scheduler{


        //
    public:
        static std::shared_ptr<Scheduler> createScheduler(){
            std::shared_ptr<Scheduler> ptr = std::make_shared<Scheduler>();
             coroutine::SchedulerSingleton<Scheduler>::Instance(ptr.get());
            return ptr;
        }
        static Scheduler* createScheduler(Scheduler* ptr){
            return coroutine::SchedulerSingleton<Scheduler>::Instance(ptr);

        }
        static void Start(){
            auto m_global_schedual_sinleton_instance = coroutine::SchedulerSingleton<Scheduler>::getInstance();
            m_global_schedual_sinleton_instance->Run();

        }
        Scheduler();
        ~Scheduler(){
            std::cout << "Scheduler Destroyed"<< std::endl;
//            coroutine::SchedulerSingleton<coroutine::Scheduler>::DestroyInstance();

        }
        void Run();

        bool addTask(co_out<int64_t> && co_ro);

    private:
        int64_t m_current_time;
        int m_invalid_num = 0;
        std::vector<std::pair<co_out<int64_t>, int64_t>> m_schedual_coroutines;
        std::mutex m_schedual_coroutines_mtx;
        std::atomic_bool m_start_run;
        double idle_spin_rate = 10.0;


    };

    template <typename T>
    class Channel{
    public:
        Channel(size_t capacity = 1):m_capacity(capacity){

            m_deque = std::make_shared<std::deque<T>>();
        }

        Channel(const Channel &chan){
            m_deque = chan.m_deque;
            m_capacity = chan.m_capacity;
        }

        Channel& operator=(const Channel &chan){
            m_deque = chan.m_deque;
            m_capacity = chan.m_capacity;
            return *this;
        }

        bool tryPush(const T& data) const {

//            std::unique_lock<std::mutex> locker(m_mtx);

            if(m_deque->size()>=m_capacity){

                return false;
            }

            m_deque->push_back(data);

            return true;
        }
        bool tryPop(T& data) const {
//            std::unique_lock<std::mutex> locker(m_mtx);
            if(m_deque->empty()){

                return false;
            }
            data = m_deque->front();
            m_deque->pop_front();

            return true;
        }
    protected:
        std::shared_ptr<std::deque<T>> m_deque;
        size_t m_capacity;
        std::mutex m_mtx;

    };



}
#endif //COROUTINE_COROUTINE_H
