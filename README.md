A simple coroutine library based on Boost.coroutine2

usege:
    
```
auto scheduler_ptr = coroutine::Scheduler::createScheduler();


coroutine::Channel<int> chan(15);

__co_create([&,chan](auto& co_yied){
    while(common::OK()){

        for(int i = 0; i < 20;i++){
            chan.tryPush(i);
            std::cout << "** tryPush chan : " << i << "\n";
            __co_sleep_for(1000);
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
                std::cout << "** tryPop chan " << b << "\n";
            }

            __co_sleep_for(1000);
            co_yied(0);

        }
        break;
    }
});

coroutine::Scheduler::Start();
```
    
    
    


1. system:
- ubuntu 16.04
- cmake version 3.14.2
- gcc version 5.4.0
- Boost version 1.7.1   
* * *
    
2. build
```
mkdir build &&cd build
cmake ..  -DBOOST_BUILD_PATH=../third_party/boost-cmake -DFETCHCONTENT_SOURCE_DIR_BOOST=path_to_boost -DCMAKE_INSTALL_PREFIX=path_to_install
make -j4
```

* * *
3. install
```
make -j4 install
```

* * *
4. pack deb
```
capck
```

* * *
5. install deb file
```
sudo dpkg -i coroutine-0.0.1-Linux.deb
```

* * *
6. use in cmake, more test in test/exmaple folder
```
find_package(coroutine  HINTS path_to_install )
message(STATUS coroutine_LIBRARIES ${coroutine_LIBRARIES})
message(STATUS coroutine_INCLUDE_DIRS ${coroutine_INCLUDE_DIRS})
```





