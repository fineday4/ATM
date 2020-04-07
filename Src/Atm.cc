/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 08:55:19 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 06:17:25
 */
#include "../Include/Atm.h"

//! 角色模型：系统中有多个离散的角色（均运行在独立的线程上），用来发送消息以完成自己的任务，除了直接通过消息传递的状态外，没有任何共享状态。
//* Atm的角色是流程控制者，其逻辑机制适合用状态机实现



void Atm::process_withdrawal()
{
    incoming.wait().handle<withdraw_ok>(
        [&](withdraw_ok const &msg)
        {
            interface_hardware.send(issue_money(withdrawal_amount));
            bank.send(withdrawal_processed(account, withdrawal_amount));
            state=&Atm::done_processing;
        }
    )
    .handle<withdraw_denied>(
        [&](withdraw_denied const &msg)
        {
            interface_hardware.send(display_insufficient_funds());
            state = &Atm::done_processing;
        }
    )
    .handle<cancel_pressed>(
        [&](cancel_pressed const &msg)
        {
            bank.send(
                cancel_withdrawal(account, withdrawal_amount)
            );
            interface_hardware.send(
                display_withdrawal_cancelled()
            );
            state = &Atm::done_processing;
        }
    );
}

void Atm::process_balance()
{
    incoming.wait()
    .handle<balance>(
        [&](balance const &msg)
        {
            interface_hardware.send(display_balance(msg.amount));
            state = &Atm::wait_for_action;
        }
    )
    .handle<cancel_pressed>(
        [&](cancel_pressed const &msg)
        {
            state = &Atm::done_processing;
        }
    );
}

void Atm::wait_for_action()
{
    interface_hardware.send(display_withdrawal_options());
    incoming.wait()
    .handle<withdraw_pressed>(
        [&](withdraw_pressed const &msg)
        {
            withdrawal_amount = msg.amount;
            bank.send(withdraw(account, msg.amount, incoming));
            state = &Atm::process_withdrawal;
        }
    )
    .handle<balance_pressed>(
        [&](balance_pressed const &msg)
        {
            bank.send(get_balance(account, incoming));
            state = &Atm::process_balance;
        }
    )
    .handle<cancel_pressed>(
        [&](cancel_pressed const &msg)
        {
            state = &Atm::done_processing;
        }
    );
}

void Atm::verifying_pin()
{
    incoming.wait()
    .handle<pin_verified>(
        [&](pin_verified const &msg)
        {
            state = &Atm::wait_for_action;
        }
    )
    .handle<pin_incorrect>(
        [&](pin_incorrect const &msg)
        {
            interface_hardware.send(
                display_pin_incorrect_message()
            );
            state = &Atm::done_processing;
        }
    )
    .handle<cancel_pressed>(
        [&](cancel_pressed const &msg)
        {
            state = &Atm::done_processing;
        }
    );
}

void Atm::getting_pin()
{
    incoming.wait()
    .handle<digit_pressed>(
        [&](digit_pressed const &msg)
        {
            unsigned const pin_length = 4;
            pin += msg.digit;
            if(pin.length() == pin_length)
            {
                bank.send(verify_pin(account, pin, incoming));
                state = &Atm::verifying_pin;
            }
        }
    )
    .handle<clear_last_pressed>(
        [&](clear_last_pressed const &msg)
        {
            if(!pin.empty()){
                pin.pop_back();
            }
        }
    )
    .handle<cancel_pressed>(
        [&](cancel_pressed const &msg)
        {
            state = &Atm::done_processing;
        }
    );
}

void Atm::waitting_for_card()
{
    interface_hardware.send(display_enter_card());
    incoming.wait()
    .handle<card_inserted>(
        [&](card_inserted const &msg)
        {
            account = msg.account;
            pin = "";
            interface_hardware.send(display_enter_pin());
            state = &Atm::getting_pin;
        }
    );
}

void Atm::done_processing()
{
    interface_hardware.send(eject_card());
    state = &Atm::waitting_for_card;
}
