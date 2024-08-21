/** 
 * @file factroy.h
 * @brief handler factory
 * @author yingxue
 * @date 2023-11-06
 */

#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include "expression.h"
#include "handler.h"
//#include "httphandler.h"

template <class Base>
class IBaseProduct {
public:
    virtual Base* CreateProduct() = 0;

protected:
    IBaseProduct() {
    }

    virtual ~IBaseProduct() {
    }

private:
    IBaseProduct(const IBaseProduct&);
    const IBaseProduct& operator=(const IBaseProduct&);
};

template <class Base>
class ProductFactory {
public:
    static ProductFactory<Base>& Instance() {
        static ProductFactory<Base> instance;
        return instance;
    }

    void RegisterProduct(const std::string& name, IBaseProduct<Base>* base) {
        m_product_map[name] = base;
    }

    Base* GetProduct(std::string name, void* userdata) {
        if (m_product_map.find(name) != m_product_map.end()) {
            return m_product_map[name]->CreateProduct();
        } 
        std::cout << "No product found for " << name << std::endl;
        return NULL;
    }
private:
    ProductFactory() {
    }
    ~ProductFactory() {
    }
    ProductFactory(const ProductFactory&);
    const ProductFactory& operator=(const ProductFactory&);

    std::map<std::string, IBaseProduct<Base>*> m_product_map;

};

template <class Base, class Super>
class ISuperProduct : public IBaseProduct<Base> {
public:
    explicit ISuperProduct(const std::string& name) {
        ProductFactory<Base>::Instance().RegisterProduct(name, this);
    }

    Base* CreateProduct() {
        return new Super();
    }
};

class MethodFactory {
public:
    static int64_t Create(const std::string& name, void* userdata) {
        std::string::size_type pos = name.find("_", 0);
        std::string classname = name.substr(0, pos);
        HandlerNode* node = ProductFactory<HandlerNode>::Instance().GetProduct(classname, userdata);
        return reinterpret_cast<int64_t>(node);
    }

    static int64_t Handle(const std::string& name, int64_t handler, int64_t action, void* userdata) {
        int64_t ret = -1;
        HandlerNode* node = reinterpret_cast<HandlerNode*>(handler);
        if (node) {
            ret = node->Handle(name, action, userdata);
        }
        return ret;
    }

    static int64_t Async(const std::string& name, int64_t handler, int64_t action, void* userdata) {
        int64_t ret = -1;
        HandlerNode* node = reinterpret_cast<HandlerNode*>(handler);
        if (node) {
            ret = node->Async(name, action, userdata);
        }
        return ret;
    }

    static int64_t Wait(const std::string& name, int64_t handler, int64_t action, int64_t timeout, void* userdata) {
        int64_t ret = -1;
        HandlerNode* node = reinterpret_cast<HandlerNode*>(handler);
        if (node) {
            ret = node->Wait(name, action, timeout, userdata);
        }
        return ret;
    }

    static std::string Get(const std::string& name, int64_t handler, const std::string& param, void* userdata) {
        std::string ret;
        HandlerNode* node = reinterpret_cast<HandlerNode*>(handler);
        if (node) {
            ret = node->Get(name, param, userdata);
        }
        return ret;
    }

    static int64_t Destroy(const std::string& name, int64_t handler, void* userdata) {
        HandlerNode* node = reinterpret_cast<HandlerNode*>(handler);
        delete node;
        return 1;
    }
};

#endif // HANDLER_H
