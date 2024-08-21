/** 
 * @file yxhelper.cc
 * @brief yxlang helper
 * @author yingxue
 * @date 2023-12-11
 */

#include "yxhelper.h"

ISuperProduct<HandlerNode, CHttpHandlerNode> CYxHelper::httpNode("http");

int64_t CYxHelper::Process(yxreq *req, yxresp *resp) {
    YxlangContext yxlang;
    Context userdata{req, resp};
    yxlang.userdata = &userdata;
    yxlang::Driver driver(&yxlang);
    std::string yxhelper_flowconfig = "flow.yx";
    std::fstream infile(yxhelper_flowconfig);

    bool result = driver.parse_stream(infile, yxhelper_flowconfig);
    if (result) {
        for (unsigned int ei = 0; ei < yxlang.expressions.size(); ++ei) {
            yxlang.expressions[ei]->evaluate();
        }
    }
}
