/** 
 * @file yxhelper.h
 * @brief yxlang helper
 * @author yingxue
 * @date 2023-12-11
 */

#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "../yxlang/driver.h"
#include "../yxlang/expression.h"
#include "../yxlang/handler.h"
#include "../yxlang/factory.h"
#include "../yxlang/coro/io_context.h"
#include "context.h"
#include "httphandler.h"

class CYxHelper {
public:
    static int64_t Process(yxreq *req, yxresp *resp);
private:
    static ISuperProduct<HandlerNode, CHttpHandlerNode> httpNode;
};
