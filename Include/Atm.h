/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-08 06:11:46 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 07:17:27
 */
#pragma once

#include "Receiver.h"
#include "withdraw.h"

//! 角色模型：系统中有多个离散的角色（均运行在独立的线程上），用来发送消息以完成自己的任务，除了直接通过消息传递的状态外，没有任何共享状态。
//* Atm的角色是流程控制者，其逻辑机制适合用状态机实现



class Atm
{
private:
    /* data */
    MESSAGE::Receiver incoming;
    MESSAGE::Sender bank;
    MESSAGE::Sender interface_hardware;
    void (Atm::*state)();  //* 状态机的状态表示，同时也是函数名
    std::string account;
    unsigned withdrawal_amount;
    std::string pin;

    void process_withdrawal();

    void process_balance();

    void wait_for_action();

    void verifying_pin();

    void getting_pin();

    void waitting_for_card();

    void done_processing();

    Atm(Atm const&) = delete;
    Atm& operator=(Atm const&) = delete;

public:
    Atm(MESSAGE::Sender bank_, MESSAGE::Sender interface_hardware_):
    bank(bank_), interface_hardware(interface_hardware_)
    {}

    void done()
    {
        get_sender().send(MESSAGE::Close_queue());
    }

    void run()
    {
        state = &Atm::waitting_for_card;
        try{
            for(;;){
                (this->*state)();
            }
        }catch(MESSAGE::Close_queue const &){
        }
    }

    MESSAGE::Sender get_sender()
    {
        return incoming;
    }

    ~Atm()
    {}
};
