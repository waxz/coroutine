//
// Created by waxz on 2020/5/21.
//

#include "coroutine.h"
namespace coroutine{


    Scheduler::Scheduler(){
        m_schedual_coroutines.reserve(20);
        m_start_run = false;
    }


    bool Scheduler::addTask(co_out<int64_t> && co_ro){

        std::cout << "Scheduler::addTask try to get lock\n";
        if(m_start_run){
            std::cout << "Scheduler is running, fail to add task\n";
            return false;
        }

        std::unique_lock<std::mutex> locker(m_schedual_coroutines_mtx);

        m_schedual_coroutines.push_back(std::make_pair(std::move(co_ro), common::ToUniversal(common::FromUnixNow())));
    }


    void Scheduler::Run(){

        if(!m_start_run){
            std::cout << "Scheduler start run\n";

            m_start_run = true;

        }else{
            std::cout << "Scheduler is running, return\n";

            return;
        }

        int valid_num = 0;

        common::Rate spin_rate(idle_spin_rate);

        while(common::OK()){
            m_invalid_num = 0;

            for(auto & i: m_schedual_coroutines){
                // update current time
                // check next running time
                m_current_time = common::ToUniversal(common::FromUnixNow());
                if(i.first) {
                    i.second = i.first.get();
                    if(m_current_time > i.second){

//                        std::cout << "not sleep run\n";
                        i.first();
                    }else{
//                        std::cout << "sleep, skip\n";

                    } }else{
                    m_invalid_num++;
//                    std::cout << " not a valid coutine, skip\n";

                }
            }

//            std::cout << "m_invalid_num: " << m_invalid_num << "\n";

            valid_num = m_schedual_coroutines.size() - m_invalid_num;

            if(1 == valid_num){
                // only one coroutine
                spin_rate.sleep();
            }

            if(m_invalid_num == m_schedual_coroutines.size()){
                std::cout << "all coroutine done"<< std::endl;
                m_start_run = false;

                break;

            }
        }

    }

}