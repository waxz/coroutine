//
// Created by waxz on 2020/5/21.
//
#include "coroutine/coroutine.h"



void test1(){
    std::shared_ptr<coroutine::Scheduler> scheduler_ptr = std::make_shared<coroutine::Scheduler>();
    int x= 9, y =3;

    scheduler_ptr->addTask(

            coroutine::co_out<int64_t>{[&](auto &co_yied) {
                while (common::OK()) {
                    std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
                    __co_sleep_for(10000);

//                    break;
                }

            }}

    );
    scheduler_ptr->Run();

}

void test2(){
    auto scheduler_ptr = coroutine::Scheduler::createScheduler();

    int x= 9, y =3;

    scheduler_ptr->addTask(

            coroutine::co_out<int64_t>{[&](auto &co_yied) {
                while (common::OK()) {
                    std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
                    __co_sleep_for(10000);

//                    break;
                }

            }}

    );

    coroutine::Scheduler::Start();
//    coroutine::SchedulerSingleton<coroutine::Scheduler>::DestroyInstance();

}

void test3(){
    std::shared_ptr<coroutine::Scheduler> scheduler_ptr = std::make_shared<coroutine::Scheduler>();

    coroutine::Scheduler::createScheduler(scheduler_ptr.get());
//    auto m_global_schedual_sinleton = coroutine::SchedulerSingleton<coroutine::Scheduler>::Instance(scheduler_ptr.get());

    int x= 9, y =3;
#if 0
    scheduler_ptr->addTask(

            coroutine::co_out<int64_t>{[&](auto &co_yied) {
                while (common::OK()) {
                    std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
                    __co_sleep_for(10000);

//                    break;
                }

            }}

    );

#endif

    {
        coroutine::Channel<int> chan(15);

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                for(int i = 0; i < 20;i++){
                    chan.tryPush(i);
                    std::cout << "** tryPush chan : " << i << "\n";
//                __co_sleep_for(1000);
                    co_yied(0);

                }
                break;
            }

        });

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                int b = 0;

                for(int i = 0; i < 20;i++){

                    bool done = chan.tryPop(b);
                    if(done){
                        std::cout << "**3 get chan " << b << "\n";

                    }


//                __co_sleep_for(1000);
                    co_yied(0);

                }


                break;
            }

        });


    }


    __co_create([&](auto& co_yied){
        while(common::OK()){
            std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
            __co_sleep_for(1000);

            break;
        }

    });


    scheduler_ptr->Run();

}


void test4(){
    auto scheduler_ptr = coroutine::Scheduler::createScheduler();

    int x= 9, y =3;

    coroutine::Channel<int> chan(5);

    __co_create([&](auto& co_yied){
        while(common::OK()){

            for(int i = 0; i < 20;i++){
                chan.tryPush(i);
                std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
//                __co_sleep_for(1000);
                co_yied(0);

            }


            break;
        }

    });

    coroutine::Scheduler::Start();

}

void test_chan(){

    coroutine::Channel<int> chan(5);
    [=]{
        std::cout << "push"<< std::endl;
        for(int i = 0; i < 20;i++){
            chan.tryPush(i);

        }

    }();

    [=]{
        std::cout << "pop"<< std::endl;
        int b;
        bool done = true;
        while(done){
            done = chan.tryPop(b);
            if(done){
                std::cout << "get: " << b << std::endl;
            }
        }

    }();

}

void test5(){
    {
        coroutine::Channel<int> chan(15);

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                for(int i = 0; i < 20;i++){
                    chan.tryPush(i);
                    std::cout << "** 55tryPush chan : " << i << "\n";
//                __co_sleep_for(1000);
                    co_yied(0);

                }
                break;
            }

        });

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                int b = 0;

                for(int i = 0; i < 20;i++){

                    bool done = chan.tryPop(b);
                    if(done){
                        std::cout << "** 55get chan " << b << "\n";

                    }


//                __co_sleep_for(1000);
                    co_yied(0);

                }


                break;
            }

        });


    }
}

void test6(){
    {
        coroutine::Channel<int> chan(15);

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                for(int i = 0; i < 20;i++){
                    chan.tryPush(i);
                    std::cout << "** 66tryPush chan : " << i << "\n";
//                __co_sleep_for(1000);
                    co_yied(0);

                }
                break;
            }

        });

        __co_create([&,chan](auto& co_yied){
            while(common::OK()){

                int b = 0;

                for(int i = 0; i < 20;i++){

                    bool done = chan.tryPop(b);
                    if(done){
                        std::cout << "** 66get chan " << b << "\n";

                    }


//                __co_sleep_for(1000);
                    co_yied(0);

                }


                break;
            }

        });


    }
}
int main(){


    common::initSystemSignalHandler();

//    test1(); //ok
//    test2(); // ok
//    test3(); //ok
//    test4(); //ok

//    test_chan(); //ok



    auto scheduler_ptr = coroutine::Scheduler::createScheduler();

    test5();
    test6();


    coroutine::Scheduler::Start();
    test6();

    return 1;

//    coroutine::Scheduler scheduler;

//    auto m_global_schedual_sinleton = coroutine::SchedulerSingleton<coroutine::Scheduler>::Instance(scheduler_ptr.get());


#if 0
    __co_create([&](auto& co_yied){
        while(common::OK()){
            std::cout << "===============check2 co_create x : " << x << ", y: " << y << "\n";
            __co_sleep_for(10000);

            break;
        }

    });
#endif


//    coroutine::Scheduler::Start();


}