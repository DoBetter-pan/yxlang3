/** 
 * @file driver.cc
 * @brief Core driver
 * @author yingxue
 * @date 2023-10-26
 */

#include <fstream>
#include <sstream>
#include "driver.h"
#include "scanner.h"

namespace yxlang {

Driver::Driver(class YxlangContext* _context) : trace_scanning(false), trace_parsing(false), context(_context) {
}

bool Driver::parse_stream(std::istream& in, const std::string& sname) {
    streamname = sname;

    Scanner scanner(&in);
    scanner.set_debug(trace_scanning);
    this->lexer = &scanner;

    Parser parser(*this);
    parser.set_debug_level(trace_parsing);
    return (parser.parse() == 0);
}

bool Driver::parse_file(const std::string &filename) {
    std::ifstream in(filename.c_str());
    if (!in.good()) return false;
    return parse_stream(in, filename);
}

bool Driver::parse_string(const std::string &input, const std::string& sname) {
    std::istringstream iss(input);
    return parse_stream(iss, sname);
}

void Driver::error(const class location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void Driver::error(const std::string& m) {
    std::cerr << m << std::endl;
}

} // namespace yxlang
