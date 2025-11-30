#pragma once
#include<vector>
#include<deque>
#include<memory>
#include<atomic>
#include<mutex>
#include<thread>
#include"TaskBase.h"
using namespace std;

class TaskManager{
private:
    deque<atomic<int>> a;
    atomic<int> out;
    vector<unique_ptr<TaskBase>> tasks;
    mutex mtx;
    thread t1,t2;
    atomic<bool> running;
public:
    TaskManager();
    ~TaskManager();

    void addTask(int key,int kind);
    void popTask();
    void callback(int key,int msg);
    void run();
    bool isRunning() const {return running.load();}
};