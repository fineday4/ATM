/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 10:18:39 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 07:09:32
 */
#pragma once

#include "Receiver.h"
#include "withdraw.h"


using namespace MESSAGE;

class Bank_machine
{
private:
    /* data */
    Receiver incoming;
    unsigned balance;

public:
    Bank_machine(/* args */):
    balance(199)
    {}

    MESSAGE::Sender get_sender()
    {
        return incoming;
    }

    void done()
    {
        get_sender().send(Close_queue());
    }

    void run();
    ~Bank_machine(){}
};
