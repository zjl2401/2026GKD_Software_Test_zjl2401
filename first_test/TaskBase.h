#pragma once
#include<iostream>
#include<thread>
#include<atomic>
using namespace std;

class TaskBase{
protected:
    int key;
    atomic<int>* p_in;
    atomic<int>* p_out;
    atomic<bool> running;
    thread th;
public:
    TaskBase(int k,atomic<int>* pin,atomic<int>* pout);
    virtual ~TaskBase();

    void start();
    void stop();
    
    virtual void run()=0;
    virtual void callback(int msg)=0;
    int getKey() const{return key;}
};

//TaskFilter
class TaskFilter:public TaskBase{
public:
    TaskFilter(int k,atomic<int>* pin,atomic<int>* pout);
    void run() override;
    void callback(int msg) override;
};

//TaskGain
class TaskGain:public TaskBase{
    atomic<int> kFactor;
public:
    TaskGain(int k,atomic<int>* pin,atomic<int>* pout);
    void run() override;
    void callback(int msg) override;
};

//TaskDelayBuffer
class TaskDelayBuffer:public TaskBase{
public:
    TaskDelayBuffer(int k,atomic<int>* pin,atomic<int>* pout);
    void run() override;
    void callback(int msg) override;
};