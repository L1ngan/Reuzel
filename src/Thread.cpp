//
// Thread.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Thread.h"
#include <assert.h>
#include <stdio.h>

using namespace Reuzel;

static void *startThread(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    thread->runInThread();
    return NULL;
}

Thread::Thread(const ThreadFunc &func)
  : started_(false),
    joined_(false),
    pthreadId_(0)
{
    func_ = func;
}

Thread::~Thread()
{
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    if (pthread_create(&pthreadId_, NULL, startThread, this) != 0) {
        started_ = false;
        printf("Faileed in pthread_create\n");
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

void Thread::runInThread()
{
    try {
        func_();
    }
    catch (const std::exception &e) {
        printf("exception caught in Thread %ld\n", pthreadId_);
        abort();
    }
}
