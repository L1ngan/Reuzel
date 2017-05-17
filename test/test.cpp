//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../src/ThreadPool.h"
#include <stdio.h>
#include <pthread.h>
#include <string>
#include <unistd.h>

void print()
{
    printf("current pthread = %ld\n", pthread_self());
}

void printString(const std::string &taskId)
{
    printf("current pthread = %ld %s\n", pthread_self(), taskId.c_str());
    usleep(10 * 1000);
}

void test(int maxSize)
{
    printf("Test ThreadPool with max queue size = %d\n", maxSize);
    Reuzel::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    printf("Adding print task\n");
    pool.addTask(print);
    pool.addTask(print);

    printf("Adding printString tast\n");
    for (int i = 0; i < 100; ++i) {
        std::string taskId("task - ");
        taskId += std::to_string(i);
        pool.addTask(std::bind(printString, taskId));
    }
    sleep(2);  // Wait until all tasks are taken out and executed.

    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}
