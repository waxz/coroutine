#ifndef COROUTINE_SYSTEM_SIGNAL_H
#define COROUTINE_SYSTEM_SIGNAL_H

#include <signal.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>

namespace common {

    inline bool &getSystemTerminalFlag() {
        static bool SystemTerminalFlag = true;
        return SystemTerminalFlag;
    }

    inline void my_handler(int signum) {
        std::cerr << "  Caught signal: " << signum << std::endl;
        if (signum == 2) {
            bool &flag = getSystemTerminalFlag();
            flag = false;
        }
    }

    static bool OK() {
        return getSystemTerminalFlag();
    }

    class SystemSignalHandler {
    public:
        struct sigaction sigIntHandler;

        SystemSignalHandler() {

            sigIntHandler.sa_handler = my_handler;
            sigemptyset(&sigIntHandler.sa_mask);
            sigIntHandler.sa_flags = 0;

            sigaction(SIGINT, &sigIntHandler, nullptr);
        }

        static bool OK() {
            return getSystemTerminalFlag();
        }
    };

    inline void initSystemSignalHandler() {
        static SystemSignalHandler s;
    }

    inline void spin() {
        std::mutex mtx;
        std::unique_lock<std::mutex> locker(mtx);
        std::condition_variable cv;
        std::chrono::duration<int, std::ratio<1, 1000>> time_count(1000);

        while (common::SystemSignalHandler::OK()) {
            cv.wait_for(locker, time_count, [] {
                return !common::SystemSignalHandler::OK();
            });
        }
    }
} // namespace common

#endif