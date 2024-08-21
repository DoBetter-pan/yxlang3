#include <iostream>
#include <thread>
#include "yxhelper/context.h"
#include "yxhelper/yxhelper.h"

void testYxHelper(int tid) {
    yxreq req;
    req.uri = std::string("hello-") + std::to_string(tid);
    yxresp resp;
    CYxHelper::Process(&req, &resp);
}

int main() {

    std::thread t1(testYxHelper, 1);
    std::thread t2(testYxHelper, 2);
    std::thread t3(testYxHelper, 3);
    std::thread t4(testYxHelper, 4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
