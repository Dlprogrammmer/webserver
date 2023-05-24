#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<pthread.h>
#include<list>
#include"locker.h"
#include<exception>
#include<cstdio>
template<typename T>
class threadpool
{
private:
    //线程的数量
    int m_thread_number;
    //线程池大小
    pthread_t *m_threads;
    //请求队列最多等待处理数量
    int m_max_requests;
    //工作队列
    std::list<T*> m_workqueue;
    //互斥锁
    locker m_queuelocker;
    //信号量判断是否有任务处理
    sem m_queuestat;
    //是否结束线程
    bool m_stop;
    static void* worker(void* arg){
        threadpool* pool=(threadpool*) arg;
        pool->run();
        return pool;
    }
public:
    threadpool(int tread_number=8,int max_request=10000):m_thread_number(tread_number),m_max_requests(max_request),m_stop(false),m_threads(NULL){
        if((tread_number<=0||max_request<=0)){
            
        }
        m_threads=new pthread_t[m_thread_number];
        if(!m_threads){
            throw std::exception();
        }
        for(int i=0;i<m_thread_number;i++)
        {
            printf("create the %th thread\n",i);
            if(pthread_create(m_threads+i,NULL,worker,this)!=0)
            {
                delete [] m_threads;
                throw std::exception();
            }
            if(pthread_detach(m_threads[i])){
                delete[] m_threads;
                throw std::exception();
            }
        }
    }
    ~threadpool(){
        delete m_threads;
        m_stop=true;
    }
    bool append(T* request){
        m_queuelocker.lock();
        if(m_workqueue.size()>m_max_requests)
        {
            m_queuelocker.unlock();
            return false;
        }
        m_workqueue.push_back(request);
        m_queuelocker.unlock();
        m_queuestat.post();
        return true;
    }
    void run()
    {
        while(!m_stop){
            m_queuestat.wait();
            m_queuelocker.lock();
            if(m_workqueue.empty()){
                m_queuelocker.unlock();
                continue;
            }
            T* request=m_workqueue.front();
            m_workqueue.push_front();
            m_queuelocker.unlock();
            if(!request)
            continue;
            request.process();
        }
    }
};

#endif