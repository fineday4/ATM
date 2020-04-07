/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 10:59:32 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 06:55:28
 */
#pragma once

#include <iostream>

#include "Receiver.h"
#include "withdraw.h"

static std::mutex iom;

class Interface_machine
{
private:
    /* data */
    MESSAGE::Receiver incoming;
public:

    void done()
    {
        get_sender().send(MESSAGE::Close_queue());
    }
    
    void run();

    MESSAGE::Sender get_sender()
    {
        return incoming;
    }
};