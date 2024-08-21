/** 
 * @file handler.h
 * @brief http handler
 * @author yingxue
 * @date 2023-11-06
 */

#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../yxlang/handler.h"
#include "../yxlang/cohttp.h"

#include "context.h"

class CHttpHandlerNode : public HandlerNode {
    int64_t	value;
    
public:
    explicit CHttpHandlerNode(int64_t _value) : HandlerNode(), value(_value) {
        count = 0;
    }

    explicit CHttpHandlerNode() : HandlerNode() {
        count = 0;
    }

    ~CHttpHandlerNode() {
    std::vector<CoHttp*>     cohttps;
        for (auto h : cohttps) {
            delete h;
        }
    }

    virtual int64_t Handle(const std::string& name, int64_t action, void* userdata) {
        int64_t v = 1;
        Context* t = reinterpret_cast<Context*>(userdata);
        if (action == CONFIG_PARSE) {
            std::cout << "CONFIG_PARSE" << std::endl;
        } else if (action == CONFIG_UPDATE) {
            std::cout << "CONFIG_UPDATE" << std::endl;
            v = 0;
        } else if (action == USER_TAG) {
            std::cout << "USER_TAG" << std::endl;
            v = 0;
        }

        /*Test Case*/
        std::string uri = t->req->uri;
        t->testcase = uri + " world!";

        std::cout << "http process:" << action << ", tid:" << uri << ", pointer:" << this << std::endl;
        return v;
    }

    virtual int64_t Async(const std::string& name, int64_t action, void* userdata) {
        int64_t v = -1;
        if (action == 3) {
            CoHttp* http = new CoHttp{"127.0.0.1", 9090};
            if (http) {
                count++;
                std::string request = "GET   /   HTTP/1.1\r\nAccept:   */*\r\nAccept-Language:   en\r\nUser-Agent:   Mozilla/4.0   (compatible;   MSIE   5.01;   Windows   NT   5.0)\r\nHost:   127.0.0.1:9090\r\nConnection:   Close\r\n\r\n";
                http->setRequestData(request);
                http->requestToServer(this);
                // cohttps.push_back(http);
            }
        } else if (action == 4) {
            CoHttp* http = new CoHttp{"127.0.0.1", 80};
            if (http) {
                count++;
                std::string request = "GET   /   HTTP/1.1\r\nAccept:   */*\r\nAccept-Language:   en\r\nUser-Agent:   Mozilla/4.0   (compatible;   MSIE   5.01;   Windows   NT   5.0)\r\nHost:   127.0.0.1:9090\r\nConnection:   Close\r\n\r\n";
                http->setRequestData(request);
                http->requestToServer(this);
                // cohttps.push_back(http);
            }
        }
        std::cout << "http async:" << action << std::endl;
        return v;
    }

    virtual int64_t Wait(const std::string& name, int64_t action, int64_t timeout, void* userdata) {
        int64_t v = 0;
        std::unique_lock<std::mutex> lck(mtx);
        while(count > 0) {
            cond.wait(lck);
        }
        std::cout << "http wait, action:" << action << ",time:" << timeout << std::endl;
        return v;
    }

    virtual std::string Get(const std::string& name, const std::string& param, void* userdata) {
        std::string v = "hello";
        Context* t = reinterpret_cast<Context*>(userdata);
        if (param == "uri") {
            std::cout << "http get:" << "test.aty.sohu.com/m" << std::endl;
            //v = "test.aty.sohu.com/m";
            //v = t->name;
        } else if (param == "args.pt") {
            std::cout << "http get:" << "oad" << std::endl;
            //v = "oad";
        }
        std::cout << "http get:" << param << ", value:" << v << std::endl;
        return v;
    }

    virtual void SetHttpStatus(CoHttp* http, int64_t code, std::string& data) {
        std::unique_lock<std::mutex> lck(mtx);
        count--;
        cond.notify_all();
        std::cout << "SetHttpStatus, code:" << code << ",data:" << data << std::endl;
        delete http;
    }

private:
    std::vector<CoHttp*>     cohttps;
    std::condition_variable  cond;
    std::mutex               mtx;
    int64_t                  count;
};

#endif // HANDLER_H
