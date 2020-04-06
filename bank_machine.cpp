/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 10:18:39 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:59:16
 */

#include "receiver.h"
#include "withdraw.h"


using namespace messaging;

class bank_machine
{
private:
    /* data */
    receiver incoming;
    unsigned balance;

public:
    bank_machine(/* args */):
    balance(199)
    {}

    messaging::sender get_sender()
    {
        return incoming;
    }

    void done()
    {
        get_sender().send(close_queue());
    }

    void run()
    {
        try{
            for(;;){
                incoming.wait()
                .handle<verify_pin>(
                    [&](verify_pin const &msg){
                        if(msg.pin == "1937"){
                            msg.atm_queue.send(pin_verified());
                        }else{
                            msg.atm_queue.send(pin_incorrect());
                        }
                    }
                )
                .handle<withdraw>(
                    [&](withdraw const &msg)
                    {
                        if(balance >= msg.amount){
                            msg.atm_queue.send(withdraw_ok());
                            balance -= msg.amount;
                        }else{
                            msg.atm_queue.send(withdraw_denied());
                        }
                    }
                )
                .handle<get_balance>(
                    [&](get_balance const &msg)
                    {
                        msg.atm_queue.send(::balance(balance));
                    }
                )
                .handle<withdrawal_processed>(
                    [&](withdrawal_processed const &msg)
                    {
                    }
                )
                .handle<cancel_withdrawal>(
                    [&](cancel_withdrawal)
                    {}
                );
            }
        }catch(messaging::close_queue const &){}
    }
    ~bank_machine(){}
};
