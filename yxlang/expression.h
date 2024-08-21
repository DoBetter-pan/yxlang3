/** 
 * @file expression.h
 * @brief calc expression 
 * @author yingxue
 * @date 2023-10-26
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <variant>
#include "factory.h"
#include "handler.h"

class CNCustomFunction;
class YxlangContext;
class YxlangNode;
class YxlangVariant;


class YxlangVariant {
    using yxvariant = std::variant<int64_t, double, std::string>;
    yxvariant value;

    public:
    YxlangVariant() {
    }

    YxlangVariant(const YxlangVariant& c) {
        value = c.value;
    }

    YxlangVariant(const yxvariant& _value):value(_value) {
    }

    YxlangVariant(const int _value):value(_value) {
    }

    YxlangVariant(const int64_t _value):value(_value) {
    }

    YxlangVariant(const double _value):value(_value) {
    }

    YxlangVariant(const std::string& _value):value(_value) {
    }

    YxlangVariant& operator=(const YxlangVariant& v) {
        this->value = v.value;
        return *this;
    }

    YxlangVariant& operator=(const yxvariant& v) {
        this->value = v;
        return *this;
    }

    YxlangVariant& operator=(const int v) {
        this->value = v;
        return *this;
    }

    YxlangVariant& operator=(const int64_t v) {
        this->value = v;
        return *this;
    }

    YxlangVariant& operator=(const double v) {
        this->value = v;
        return *this;
    }

    YxlangVariant& operator=(const std::string& v) {
        this->value = v;
        return *this;
    }

    virtual ~YxlangVariant() {
    }

    double getDoubleValue() const {
        double v = 0;
        if (std::holds_alternative<int64_t>(value) ) {
            v = (double)std::get<int64_t>(value);
        } else {
            v = std::get<double>(value);
        }
        return v;
    }

    YxlangVariant operator+(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) + std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                double v = getDoubleValue() + b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            std::string v = std::get<std::string>(value) + std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator-() {
        if (std::holds_alternative<int64_t>(value)) {
            int64_t v = std::get<int64_t>(value);
            return YxlangVariant(-v);
        } else if (std::holds_alternative<double>(value)) {
            double v = std::get<double>(value);
            return YxlangVariant(-v);
        }
        return *this;
    }

    YxlangVariant operator-(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) - std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                double v = getDoubleValue() - b.getDoubleValue();
                return YxlangVariant(v);
            }
        }
        return *this;
    }

    YxlangVariant operator*(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) * std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                double v = getDoubleValue() * b.getDoubleValue();
                return YxlangVariant(v);
            }
        }
        return *this;
    }

    YxlangVariant operator/(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) / std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                double v = getDoubleValue() / b.getDoubleValue();
                return YxlangVariant(v);
            }
        }
        return *this;
    }

    YxlangVariant operator%(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) % std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                double v =  std::fmod(getDoubleValue(), b.getDoubleValue());
                return YxlangVariant(v);
            }
        }
        return *this;
    }

    YxlangVariant operator^(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::pow(std::get<int64_t>(value), std::get<int64_t>(b.value));
                return YxlangVariant(v);
            } else {
                double v =  std::pow(getDoubleValue(), b.getDoubleValue());
                return YxlangVariant(v);
            }
        }
        return *this;
    }

    YxlangVariant operator>(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) > std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() > b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) > std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator<(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) < std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() < b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) < std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator>=(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) >= std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() >= b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) >= std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator<=(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) <= std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() <= b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) <= std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator==(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) == std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() == b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) == std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    YxlangVariant operator!=(const YxlangVariant& b) {
        if (value.index() <= 1 && b.value.index() <= 1) {
            if (std::holds_alternative<int64_t>(value) && std::holds_alternative<int64_t>(b.value)) {
                int64_t v =  std::get<int64_t>(value) != std::get<int64_t>(b.value);
                return YxlangVariant(v);
            } else {
                int64_t v = getDoubleValue() != b.getDoubleValue();
                return YxlangVariant(v);
            }
        } else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(b.value)) {
            int64_t v = std::get<std::string>(value) != std::get<std::string>(b.value);
            return YxlangVariant(v);
        }
        return *this;
    }

    int64_t operator>(const int64_t b) {
        double v = getDoubleValue();
        return v > b;
    }

    int64_t operator<(const int64_t b) {
        double v = getDoubleValue();
        return v < b;
    }

    int64_t operator>=(const int64_t b) {
        double v = getDoubleValue();
        return v >= b;
    }

    int64_t operator<=(const int64_t b) {
        double v = getDoubleValue();
        return v <= b;
    }

    int64_t operator==(const int64_t b) {
        double v = getDoubleValue();
        return v == b;
    }

    int64_t operator!=(const int64_t b) {
        double v = getDoubleValue();
        return v != b;
    }

    int64_t operator>(const double b) {
        double v = getDoubleValue();
        return v > b;
    }

    int64_t operator<(const double b) {
        double v = getDoubleValue();
        return v < b;
    }

    int64_t operator>=(const double b) {
        double v = getDoubleValue();
        return v >= b;
    }

    int64_t operator<=(const double b) {
        double v = getDoubleValue();
        return v <= b;
    }

    int64_t operator==(const double b) {
        double v = getDoubleValue();
        return v == b;
    }

    int64_t operator!=(const double b) {
        double v = getDoubleValue();
        return v != b;
    }

    int64_t operator>(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) > b;
        }
        return v;
    }

    int64_t operator<(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) < b;
        }
        return v;
    }

    int64_t operator>=(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) >= b;
        }
        return v;
    }

    int64_t operator<=(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) <= b;
        }
        return v;
    }

    int64_t operator==(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) == b;
        }
        return v;
    }

    int64_t operator!=(const std::string& b) {
        int64_t v = -1;
        if (std::holds_alternative<std::string>(value)) {
            v = std::get<std::string>(value) != b;
        }
        return v;
    }

    operator double() {
        double v = getDoubleValue();
        return v;
    }

    operator int64_t() {
        int64_t v = 1;
        if (std::holds_alternative<int64_t>(value)) {
            v = std::get<int64_t>(value);
        }
        return v;
    }

    friend std::ostream& operator<<(std::ostream& os, const YxlangVariant& v) {
        if (std::holds_alternative<int64_t>(v.value) ) {
            os << std::get<int64_t>(v.value);
        } else if (std::holds_alternative<double>(v.value) ) {
            os << std::get<double>(v.value);
        } else if (std::holds_alternative<std::string>(v.value) ) {
            os << std::get<std::string>(v.value);
        }
        return os;
    }

};

/** Yxlang context  */
class YxlangContext {
public:
    // using variant_type = std::variant<int64_t, double, std::string>;
    using variant_type = YxlangVariant;

    typedef std::map<std::string, variant_type> variablemap_type;
    variablemap_type		variables;
    typedef std::map<std::string, CNCustomFunction*> functionmap_type;
    functionmap_type		functions;

public:
    std::vector<YxlangNode*>	expressions;

    YxlangContext() {
        userdata = NULL;
    }

    ~YxlangContext() {
        clearExpressions();
    }

    void clearExpressions();

    void *userdata;

    void setVariable(const std::string &varname, variant_type value) {
        variables[varname] = value;
    }
    bool existsVariable(const std::string &varname) const {
        return variables.find(varname) != variables.end();
    }
    variant_type getVariable(const std::string &varname) const {
        variablemap_type::const_iterator vi = variables.find(varname);
        if (vi == variables.end())
            return variant_type(0);
        else
            return vi->second;
    }

    void setFunction(const std::string &funcname, const CNCustomFunction* value) {
        functions[funcname] = const_cast<CNCustomFunction*>(value);
    }
    bool existsFunction(const std::string &funcname) const {
        return functions.find(funcname) != functions.end();
    }
    CNCustomFunction* getFunction(const std::string &funcname) const {
        functionmap_type::const_iterator vi = functions.find(funcname);
        if (vi == functions.end())
            return NULL;
        else
            return vi->second;
    }
};

/** base Yxlang node */
class YxlangNode {
public:
    // using variant_type = std::variant<int64_t, double, std::string>;
    using variant_type = YxlangVariant;

public:
    virtual ~YxlangNode() {
    }

    virtual variant_type evaluate() const = 0;

    virtual void	print(std::ostream &os, unsigned int depth=0) const = 0;
    static inline std::string indent(unsigned int d) {
        return std::string(d * 2, ' ');
    }
};

/** constant Yxlang node  */
class CNConstant : public YxlangNode {
    variant_type	value;
    
public:
    explicit CNConstant(int64_t _value) : YxlangNode(), value(_value) {
    }

    explicit CNConstant(double _value) : YxlangNode(), value(_value) {
    }

    explicit CNConstant(std::string* _value) : YxlangNode(), value(*_value) {
    }

    virtual variant_type evaluate() const {
        return value;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << value << std::endl;
    }
};

/** variable Yxlang node  */
class CNVariable : public YxlangNode {
    variant_type value;
    std::string* name;
    YxlangContext* context;

public:
    explicit CNVariable(std::string* _name, YxlangContext* _context = NULL) : YxlangNode(), name(_name), context(_context) {
    }

    virtual ~CNVariable() {
        delete name;
    }

    virtual variant_type evaluate() const {
        variant_type v = 0;
        if (context->existsVariable(*name)) {
            v = context->getVariable(*name);
        }
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << *name << ":" << evaluate() << std::endl;
    }
};

/** negate Yxlang node  */
class CNNegate : public YxlangNode {
    YxlangNode* 	node;

public:
    explicit CNNegate(YxlangNode* _node) : YxlangNode(), node(_node) {
    }

    virtual ~CNNegate() {
        delete node;
    }

    virtual variant_type evaluate() const {
        return - node->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "- negate" << std::endl;
        node->print(os, depth+1);
    }
};

/** add Yxlang node */
class CNAdd : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNAdd(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNAdd() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() + right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "+ add" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** subtract Yxlang node */
class CNSubtract : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNSubtract(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNSubtract() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() - right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "- subtract" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** multiply Yxlang node */
class CNMultiply : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNMultiply(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNMultiply() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() * right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "* multiply" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** divide Yxlang node */
class CNDivide : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNDivide(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNDivide() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() / right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "/ divide" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** modulo Yxlang node */
class CNModulo : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNModulo(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNModulo() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() % right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "% modulo" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** power Yxlang node */
class CNPower : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNPower(YxlangNode* _left, YxlangNode* _right) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNPower() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        return left->evaluate() ^ right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << "^ power" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** compare Yxlang node */
class CNCompare : public YxlangNode {
    int             fn;
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNCompare(int _fn, YxlangNode* _left, YxlangNode* _right) : YxlangNode(), fn(_fn), left(_left), right(_right) {
    }

    virtual ~CNCompare() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v(0);
        variant_type leftValue = left->evaluate();
        variant_type rightValue = right->evaluate();
        switch (fn) {
            case 1: {
                v = leftValue > rightValue;
                break;
            }
            case 2: {
                v = leftValue < rightValue;
                break;
            }
            case 3: {
                v = leftValue != rightValue;
                break;
            }
            case 4: {
                v = leftValue == rightValue;
                break;
            }
            case 5: {
                v = leftValue >= rightValue;
                break;
            }
            case 6: {
                v = leftValue <= rightValue;
                break;
            }
        }
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << fn << " compare" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** unary function Yxlang node */
class CNUnaryFunction : public YxlangNode {
    int             fn;
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNUnaryFunction(int _fn, YxlangNode* _left, YxlangNode* _right = NULL) : YxlangNode(), fn(_fn), left(_left), right(_right) {
    }

    virtual ~CNUnaryFunction() {
        delete left;
    }

    virtual variant_type evaluate() const {
        double v = 0;
        variant_type leftValue = left->evaluate();
        switch (fn) {
            case 1: {
                v = sqrt(leftValue);
                break;
            }
            case 2: {
                v = exp(leftValue);
                break;
            }
            case 3: {
                v = log(leftValue);
                break;
            }
            case 4: {
                std::cout << "= " << v << std::endl;
                v = leftValue;
                break;
            }
        }
	    return variant_type(v);
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << fn << " unaryfunction" << std::endl;
        left->print(os, depth+1);
    }
};

/** baniry function Yxlang node */
class CNBinaryFunction : public YxlangNode {
    int             fn;
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNBinaryFunction(int _fn, YxlangNode* _left, YxlangNode* _right ) : YxlangNode(), fn(_fn), left(_left), right(_right) {
    }

    virtual ~CNBinaryFunction() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type leftValue = left->evaluate();
        variant_type rightValue = right->evaluate();
        double v = 0;
        switch (fn) {
            case 1: {
                v = pow(leftValue, rightValue);
                break;
            }
        }

	    return variant_type(v);
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << fn << " binaryfunction" << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** exprlist Yxlang node */
class CNExprlist : public YxlangNode {
public:
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNExprlist(YxlangNode* _left, YxlangNode* _right = NULL) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNExprlist() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type leftValue = left->evaluate();
        //double rightValue = right->evaluate();
	    return leftValue;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " exprlist" << std::endl;
        left->print(os, depth+1);
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** assignment Yxlang node */
class CNAssignment : public YxlangNode {
    std::string* 	name;
    YxlangNode* 	left;
    YxlangNode* 	right;
    YxlangContext*  context;
    
public:
    explicit CNAssignment(std::string* _name, YxlangNode* _left = NULL, YxlangContext* _context = NULL) : YxlangNode(), name(_name), left(_left), context(_context) {
    }

    virtual ~CNAssignment() {
        delete name;
        delete left;
    }

    virtual variant_type evaluate() const {
        variant_type v = left->evaluate();
        context->setVariable(*name, v);
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " assignment:" << *name << ":" << evaluate() << std::endl;
        left->print(os, depth+1);
    }
};

/** condition Yxlang node */
class CNCondition : public YxlangNode {
    YxlangNode* 	cond;
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNCondition(YxlangNode* _cond, YxlangNode* _left, YxlangNode* _right = NULL) : YxlangNode(), cond(_cond), left(_left), right(_right) {
    }

    virtual ~CNCondition() {
        delete cond;
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v(0) ;
        variant_type k = cond->evaluate();
        if (k != (int64_t)0) {
            v = left->evaluate();
        } else {
            if (right) {
                v = right->evaluate();
            }
        }
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " condition" << std::endl;
        cond->print(os, depth+1);
        left->print(os, depth+1);
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** statement Yxlang node */
class CNStatement : public YxlangNode {
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNStatement(YxlangNode* _left, YxlangNode* _right = NULL) : YxlangNode(), left(_left), right(_right) {
    }

    virtual ~CNStatement() {
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v = left->evaluate();
        if (right) {
            v = right->evaluate();
        }
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " statement" << std::endl;
        left->print(os, depth+1);
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** paramlist Yxlang node */
class CNParamlist : public YxlangNode {
public:
    std::string* 	name;
    YxlangNode* 	left;
    YxlangNode* 	right;
    
public:
    explicit CNParamlist(std::string* _name, YxlangNode* _left,  YxlangNode* _right = NULL) : YxlangNode(), name(_name), left(_left), right(_right) {
    }

    virtual ~CNParamlist() {
        delete name;
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v = 0;
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " paramlist: " << *name << std::endl;
        if (left){
            left->print(os, depth+1);
        }
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** custom function Yxlang node */
class CNCustomFunction : public YxlangNode {
public:
    std::string*     name;
    /// paramlist
    YxlangNode* 	left;
    /// sentencelist
    YxlangNode* 	right;
    YxlangContext*  context;
    
public:
    explicit CNCustomFunction(std::string* _name, YxlangNode* _left, YxlangNode* _right, YxlangContext* _context = NULL) : YxlangNode(), name(_name), left(_left), right(_right), context(_context) {
    }

    virtual ~CNCustomFunction() {
        /// delete name;
        /// delete left;
        /// delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v = 0;
        CNCustomFunction* copy = new CNCustomFunction(name, left, right);
        context->setFunction(*name, copy);
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " function:" << *name << std::endl;
        left->print(os, depth+1);
        right->print(os, depth+1);
    }
};

/** call UDF Yxlang node */
class CNCallUDF : public YxlangNode {
    std::string*     name;
    /// exprlist
    YxlangNode* 	left;
    YxlangNode* 	right;
    YxlangContext*  context;
    
public:
    explicit CNCallUDF(std::string* _name, YxlangNode* _left, YxlangNode*  _right = NULL, YxlangContext* _context = NULL) : YxlangNode(), name(_name), left(_left), right(_right), context(_context) {
    }

    virtual ~CNCallUDF() {
        delete name;
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v = 0;
        CNCustomFunction* func = context->getFunction(*name);
        if (func) {
            std::map<std::string, variant_type> oldVal;
            CNParamlist* paramnode = dynamic_cast<CNParamlist*>(func->left);
            CNExprlist* exprnode = dynamic_cast<CNExprlist*>(left);
            while (paramnode) {
                std::string* varname = paramnode->name;
                oldVal[*varname] = context->getVariable(*varname);
                variant_type val = 0;
                if (exprnode) {
                    val = exprnode->left->evaluate();
                    exprnode = dynamic_cast<CNExprlist*>(exprnode->right);
                }
                context->setVariable(*varname, val);
                paramnode = dynamic_cast<CNParamlist*>(paramnode->left);
            }

            v = func->right->evaluate();
            /* restore old values */
            paramnode = dynamic_cast<CNParamlist*>(func->left);
            while (paramnode) {
                std::string* varname = paramnode->name;
                context->setVariable(*varname, oldVal[*varname]);
                paramnode = dynamic_cast<CNParamlist*>(paramnode->right);
            }
        }
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " call UDF:" << *name << std::endl;
        left->print(os, depth+1);
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** call method Yxlang node */
class CNMethod : public YxlangNode {
    std::string*     name;
    /// exprlist
    YxlangNode* 	left;
    YxlangNode* 	right;
    YxlangContext*  context;
    HandlerNode*    node;
    
public:
    explicit CNMethod(std::string* _name, YxlangNode* _left, YxlangNode*  _right = NULL, YxlangContext* _context = NULL) : YxlangNode(), name(_name), left(_left), right(_right), context(_context) {
    }

    virtual ~CNMethod() {
        delete name;
        delete left;
        delete right;
    }

    virtual variant_type evaluate() const {
        variant_type v = 0;
        std::vector<variant_type> params;
        CNExprlist* exprnode = dynamic_cast<CNExprlist*>(left);
        while (exprnode) {
            variant_type val = exprnode->left->evaluate();
            params.push_back(val);
            exprnode = dynamic_cast<CNExprlist*>(exprnode->right);
        }

        if (*name == "http_create" ) {
            v = MethodFactory::Create(*name, context->userdata);
        } else if (*name == "http_handle" && params.size() >= 2) {
            v = MethodFactory::Handle(*name, params[0], params[1], context->userdata);
        } else if (*name == "http_async" && params.size() >= 2) {
            v = MethodFactory::Async(*name, params[0], params[1], context->userdata);
        } else if (*name == "http_wait" && params.size() >= 2) {
            v = MethodFactory::Wait(*name, params[0], params[1], params[2], context->userdata);
        } else if (*name == "http_desdroy" && params.size() >= 1) {
            v = MethodFactory::Destroy(*name, params[0], context->userdata);
        }

        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " method:" << *name << std::endl;
        if (left) {
            left->print(os, depth+1);
        }
        if (right) {
            right->print(os, depth+1);
        }
    }
};

/** call method Yxlang node */
class CNMember : public YxlangNode {
    std::string*     handler;
    std::string*     member;
    YxlangContext*  context;
    HandlerNode*    node;
    
public:
    explicit CNMember(std::string* _handler, std::string* _member, YxlangContext* _context = NULL) : YxlangNode(), handler(_handler), member(_member), context(_context) {
    }

    virtual ~CNMember() {
        delete handler;
        delete member;
    }

    variant_type getHandler(const std::string& name) const {
        variant_type v = 0;
        if (context->existsVariable(name)) {
            v = context->getVariable(name);
        }
        return v;
    }

    virtual variant_type evaluate() const {
        variant_type h = getHandler(*handler);
        variant_type v = MethodFactory::Get(*handler, h, *member, context->userdata);
        return v;
    }

    virtual void print(std::ostream &os, unsigned int depth) const {
        os << indent(depth) << " member:" << *handler << "(" << getHandler(*handler) << ")," << *member << std::endl;
    }
};

#endif // EXPRESSION_H
