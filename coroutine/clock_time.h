//
// Created by waxz on 19-6-4.
//

#ifndef COROUTINE_CLOCK_TIME_H
#define COROUTINE_CLOCK_TIME_H

#include <chrono>
#include <ostream>
#include <ratio>
#include <thread>
#include <iostream>
#include <vector>
#include <tuple>

#include "types.h"

namespace common {

//https://codereview.stackexchange.com/questions/132852/easy-to-use-c-class-for-asking-current-time-stamp-in-milli-micro-and-nanose/132863
    class CurrentTime {
        std::chrono::high_resolution_clock m_clock;

    public:
        uint64_t milliseconds() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_clock.now().time_since_epoch()).count();
        }

        uint64_t microseconds() {
            return std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
        }

        uint64_t nanoseconds() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
        }
    };

//================================
// timer
    class Timer {
    public:
        Timer():m_StartTime (std::chrono::system_clock::now())
        {

        }
        std::vector<std::tuple<std::string, double, double>> m_time_record;



        void start() {
            m_StartTime = std::chrono::system_clock::now();
            m_bRunning = true;
        }

        double getTime() const {
            std::chrono::time_point<std::chrono::system_clock> endTime;

            if (m_bRunning) {
                endTime = std::chrono::system_clock::now();
            } else {
                endTime = m_EndTime;
            }
            return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
        }

        double microSeconds() const {

            return std::chrono::duration_cast<std::chrono::microseconds>(m_EndTime - m_StartTime).count();
        }

        double seconds() const {
            return microSeconds() / 1000000.0;
        }

        void stop() {
            m_EndTime = std::chrono::system_clock::now();
            m_bRunning = false;
            getTime();
        }

        void record(const std::string& msg){
            double t = getTime() * 0.001;

            if(m_time_record.empty()){
                m_time_record.push_back(std::make_tuple(msg,t,0.0));

            }else{
                double t0 = std::get<1>(m_time_record.back());

                m_time_record.push_back(std::make_tuple(msg,t,t - t0));

            }

        }
    private:
        std::chrono::time_point<std::chrono::system_clock> m_StartTime;
        std::chrono::time_point<std::chrono::system_clock> m_EndTime;
        bool m_bRunning = false;
    };

    inline std::ostream &operator<<(std::ostream &s, const Timer &timer) {
        s << std::endl
          << "timer record:\n";
        s << "timer count duration\n"
                    << timer.seconds() << " seconds, "
                    << timer.microSeconds() << " microSeconds"
                    << std::endl;


        if(!timer.m_time_record.empty()){
            for(auto& r: timer.m_time_record){

                std::cout <<"** msg:" <<std::get<0>(r) << ", all time: " << std::get<1>(r) << " ms, segment time: " << std::get<2>(r) << " ms" << std::endl;
            }
        }
        return s;
    }

//================================
// rate

    class Rate {
    private:
        float m_rate;
        size_t m_ms;

    public:
        Rate(float rate) : m_rate(rate), m_ms(1000 / m_rate) {};

        void sleep() {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_ms));
        };
    };

//=============================================
// from cartographer/common/time.h
////719162 是0001年1月1日到1970年1月1日所经历的天数
    constexpr int64 kUtsEpochOffsetFromUnixEpochInSeconds =
            (719162ll * 24ll * 60ll * 60ll);

    struct UniversalTimeScaleClock {
        using rep = int64;
        using period = std::ratio<1, 10000000>; // 1e-7 s = 1e-1us = 100ns
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<UniversalTimeScaleClock>;
        static constexpr bool is_steady = true;
    };

// Represents Universal Time Scale durations and timestamps which are 64-bit
// integers representing the 100 nanosecond ticks since the Epoch which is
// January 1, 1 at the start of day in UTC.
    using Duration = UniversalTimeScaleClock::duration;
    using Time = UniversalTimeScaleClock::time_point;

// Convenience functions to create common::Durations.
    Duration FromSeconds(double seconds);

    Duration FromMilliseconds(int64 milliseconds);

// Returns the given duration in seconds.
    double ToSeconds(Duration duration);

// Creates a time from a Universal Time Scale.
    Time FromUniversal(int64 ticks);

// Outputs the Universal Time Scale timestamp for a given Time.
//将c++的time_point对象转为TUC时间,单位是us
    int64 ToUniversal(Time time);

// For logging and unit tests, outputs the timestamp integer.
    std::ostream &operator<<(std::ostream &os, Time time);

// utc time <---> unix time
//https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
    void unix_epoch_count();

    Time FromUnixNow();

} // namespace common
#endif //COROUTINE_CLOCK_TIME_H
