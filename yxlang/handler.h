/** 
 * @file handler.h
 * @brief http handler
 * @author yingxue
 * @date 2023-10-30
 */

#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <cmath>

enum action {
    CONFIG_PARSE = 1,
    CONFIG_UPDATE,
    CHEATER,
    FILTER,
    USER_TAG,
    VIDEO_TAG,
    SEARCH,
    EXCLUDE,
    COUNTER,
    FREQUNCY,
    SCORE,
    SORT,
    CONFLICT,
    POSITION,
    PDB_CHECK,
    RESP
};

class CoHttp;

/** base handler node */
class HandlerNode {
public:
    typedef std::map<std::string, std::string> argsmap_type;

    std::string uri;
    argsmap_type args;
    argsmap_type cookies;
    std::string body;

    static int64_t handler_count;

public:
    virtual ~HandlerNode() {
    }

    virtual int64_t Handle(const std::string& name, int64_t action, void* userdata) = 0;
    virtual int64_t Async(const std::string& name, int64_t action, void* userdata) = 0;
    virtual int64_t Wait(const std::string& name, int64_t action, int64_t timeout, void* userdata) = 0;
    virtual std::string Get(const std::string& name, const std::string& param, void* userdata) = 0;
    virtual void SetHttpStatus(CoHttp* http, int64_t code, std::string& data) = 0;
};

#endif // HANDLER_H
