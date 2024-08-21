/** 
 * @file context.h
 * @brief context
 * @author yingxue
 * @date 2023-12-08
 */

#pragma once

#include <unistd.h>
#include <iostream>

struct yxreq {
    std::string uri;
    std::string args;
};

struct yxresp {
    int32_t status;
    std::string body;
};

class Context {
public:
    Context (yxreq *_req, yxresp *_resp): req(_req), resp(_resp) {
    }

public:
    yxreq *req;
    yxresp *resp;
    std::string testcase;
};
