#include "TaskBase.h"
#include<chrono>
using namespace std;

//TaskBase
TaskBase::TaskBase(int k,atomic<int>* pin,atomic<int>* pout)
:key(k),p_in(pin),p_out(pout),running(false){}

TaskBase::~TaskBase(){
    stop();
}

void TaskBase::start(){
    running.store(true);
    th=thread([this](){this->run();});
}

void TaskBase::stop(){
    running.store(false);
    if(th.joinable()) th.join();
}

//TaskFliter
TaskFilter::TaskFilter(int k,atomic<int>* pin,atomic<int>* pout)
:TaskBase(k,pin,pout){}

void TaskFilter::run(){
    while(running.load()){
        int val=p_in->exchange(0);
        if(val!=0){
            int result=val+1;
            *p_out=result;
            cout<<"write Filter-"<<key<<":"<<result<<endl;
        }this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void TaskFilter::callback(int msg){
    p_in->store(msg);
}

//TaskGain
TaskGain::TaskGain(int k,atomic<int>* pin,atomic<int>* pout)
:TaskBase(k,pin,pout),kFactor(1){}

void TaskGain::run(){
    while(running.load()){
        int val=p_in->exchange(0);
        if(val!=0){
            int result=val*kFactor.load();
            *p_out=result;
            cout<<"write Gain-"<<key<<":"<<result<<endl;
        }this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void TaskGain::callback(int msg){
    kFactor.store(msg);
    p_in->store(1);
}

//TaskDelayBuffer
TaskDelayBuffer::TaskDelayBuffer(int k,atomic<int>* pin,atomic<int>* pout)
:TaskBase(k,pin,pout){}

void TaskDelayBuffer::run(){
    while(running.load()){
        int val=p_in->exchange(0);
        if(val!=0){
            int t=val;
            *p_out=t;
            cout<<"write Delay-"<<key<<":"<<t<<endl;
            this_thread::sleep_for(chrono::milliseconds(1));
            *p_out=t+1;
            cout<<"write Delay-"<<key<<":"<<(t+1)<<endl;
            this_thread::sleep_for(chrono::milliseconds(1));
        }this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void TaskDelayBuffer::callback(int msg){
    p_in->store(msg);
}