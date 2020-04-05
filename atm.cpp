/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 08:55:19 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:53:37
 */
#include "receiver.h"
#include "withdraw.h"


class atm
{
private:
    /* data */
    messaging::receiver incoming;
    messaging::sender bank;
    messaging::sender interface_hardware;
    void (atm::*state)();
    std::string account;
    unsigned withdrawal_amount;
    std::string pin;

    void process_withdrawal()
    {
        incoming.wait().handle<withdraw_ok>(
            [&](withdraw_ok const &msg)
            {
                interface_hardware.send(issue_money(withdraw_amount));
                bank.send(withdrawal_processed(account, withdraw_amount));
                state=&atm::done_processing;
            }
        )
        .handle<withdraw_denied>(
            [&](withdraw_denied const &msg)
            {
                interface_hardware.send(display_insufficient_funds());
                state = &atm::done_processing;
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
                state = &atm::done_processing;
            }
        );
    }

    void process_balance()
    {
        incoming.wait()
        .handle<balance>(
            [&](balance const &msg)
            {
                interface_hardware.send(display_balance(msg.amount));
                state = &atm::wait_for_action;
            }
        )
        .handle<cancel_pressed>(
            [&](cancel_pressed const &msg)
            {
                state = &atm::done_processing;
            }
        );
    }

    void wait_for_action()
    {
        interface_hardware.send(display_withdrawal_options());
        incoming.wait()
        .handle<withdraw_pressed>(
            [&](withdraw_pressed const &msg)
            {
                withdraw_amount = msg.amount;
                bank.send(withdraw(account, msg.amount, incoming));
                state = &atm::process_withdrawal;
            }
        )
        .handle<balance_pressed>(
            [&](balance_pressed const &msg)
            {
                bank.send(get_balance(account, incoming));
                state = &atm::process_balance;
            }
        )
        .handle<cancel_pressed>(
            [&](cancel_pressed const &msg)
            {
                state = &atm::done_processing;
            }
        );
    }

    void verifying_pin()
    {
        incoming.wait()
        .handle<pin_verified>(
            [&](cancel_pressed const &msg)
            {
                state = &atm::wait_for_action;
            }
        )
        .handle<pin_incorrect>(
            [&](pin_incorrect const &msg)
            {
                interface_hardware.send(
                    display_pin_incorrect_message()
                );
                state = &atm::done_processing;
            }
        )
        .handle<cancel_pressed>(
            [&](cancel_pressed const &msg)
            {
                state = &atm::done_processing;
            }
        );
    }

    void getting_pin()
    {
        incoming.wait()
        .handle<digit_pressed>(
            [&](digit_pressed const &msg)
            {
                unsigned const pin_length = 4;
                Pin += msg.digit;
                if(pin.length() == pin.length)
                {
                    bank.send(verify_pin(account, pin, incoming));
                    state = &atm::verifying_pin;
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
                state = &atm::done_processing;
            }
        );
    }

    void waitting_for_card()
    {
        interface_hardware.send(display_enter_card());
        incoming.wait()
        .handle<card_inserted>(
            [&](card_inserted const &msg)
            {
                account = msg.account;
                pin = "";
                interface_hardware.send(display_enter_pin());
                state = &atm::getting_pin;
            }
        );
    }

    void done_processing()
    {
        interface_hardware.send(eject_card());
        state = &atm::waitting_for_card;
    }

    atm(atm const&) = delete;
    atm& operator=(atm const&) = delete;

public:
    atm(messaging::sender bank_, messaging::sender interface_hardware_):
    bank(bank_), interface_hardware(interface_hardware_)
    {}

    void done()
    {
        get_sender().send(close_queue());
    }

    void run()
    {
        state = &atm::waitting_for_card;
        try{
            for(;;){
                (this->*state)();
            }
        }catch(close_queue const &){
        }
    }

    messaging::sender get_sender()
    {
        return incoming;
    }

    ~atm()
    {}
};