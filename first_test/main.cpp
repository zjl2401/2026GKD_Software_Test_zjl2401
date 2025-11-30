#include "TaskManager.h"
#include <thread>
#include <chrono>
using namespace std;

int main() {
    TaskManager manager;
    manager.run();

    // 主线程等待子线程退出
    while (manager.isRunning()) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}