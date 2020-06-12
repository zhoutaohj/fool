#include "Thread.h"
#include <stdio.h>

//#define LOG<<## printf("##")
namespace fool
{
    std::atomic_int64_t Thread::thread_number(0);
    Thread::Thread()
    {
        init();
        thread_number++;
        thread_num_id =thread_number;
    }

     Thread::Thread(ThreadFun fun)
    {
        thread_fun=fun;
        detached =false;
        started =false;
        thread_number++;
        thread_num_id =thread_number;
    }

    Thread::Thread(Thread && t)
    {
        if(this ==&t)
            return;

        
        thread_id = t.thread_id;
        thread_fun =t.thread_fun;
        detached =t.detached;
        started =t.started;
        t.thread_id=0;



        thread_number++;
        thread_num_id =thread_number;


        

    }

    Thread& Thread::operator=(Thread&& t)
    {
        if(this ==&t)
            return *this;

        thread_id = t.thread_id;
        thread_fun =t.thread_fun;
        detached =t.detached;
        started =t.started;
    

        return *this;
    }

    Thread::~Thread()
    {
        if(!detached && started &&thread_id!=0)
        {
            join();
        }

        //printf("thread %lu exit\n",thread_num_id);

    }

    void Thread::init()
    {
        thread_id =0;
        detached =false;
        started =false;
    }

    bool Thread::start()
    {
        ThreadData*  td =new ThreadData(thread_fun); 

        int ret =pthread_create(&thread_id,NULL,run,td);

        if(ret !=0)
        {
            printf("Thread create failed!!!\n");
            return false;
        }
        //printf("Thread create succesfully!!!\n");
        started =true;
        return true;
    }

    void * Thread::run(void* param)
    {
        ThreadData * td  =reinterpret_cast<ThreadData *>(param);

        if(td->thread_fun)
        {
            td->thread_fun();
        }

        delete td;
        td= nullptr;   
        return (void*) 0;
    }

    void Thread::join()
    {
        if(started)
            pthread_join(thread_id,nullptr);

        started =false;
    }

    void Thread::detach()
    {
        if(!detached)
            pthread_detach(thread_id);

        detached =true;
    }

};
